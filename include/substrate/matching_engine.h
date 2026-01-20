/* ==============================================================================
# Substrate: Financial Execution Research Platform
# ==============================================================================
# Copyright (C) 2026  Harish Naik

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU LesserGeneral Public License along
# with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
# ============================================================================*/

/*
Reference:
https://onepagecode.substack.com/p/electronic-market-structure-and-trading
*/

#include "cancel_order.h"
#include "limit_order_book.h"
#include "logging.h"
#include "message_helpers.h"
#include "new_order.h"
#include "replace_order.h"
#include "utilities.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

namespace substrate {
template <typename PriceKey, typename LogStream>
class MatchingEngine {
public:
    void process_csv(const std::string& input)
    {
        try {
            auto request = parse_request_csv(input);

            std::visit(
                overloaded{
                    [&](const NewOrder& req) { handle_add(req); },
                    [&](const CancelOrder& req) { handle_cancel(req); },
                    [&](const ReplaceOrder& req) { handle_replace(req); }},
                request);
        } catch(std::domain_error& err) {
            DEBUG(err.what());
        } catch(std::invalid_argument&) {
            logger_.err() << "Unknown message type: " << input << std::endl;
        }
    }

private:
    using value_type = std::unique_ptr<LimitOrderBook<PriceKey, LogStream>>;
    std::unordered_map<std::string, value_type> e_;
};
} // namespace substrate