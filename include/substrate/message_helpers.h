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

#pragma once
#include "common_types.h"
#include "messages.h"
#include "string_helpers.h"
#include "substrate/cancel_order.h"
#include "substrate/new_order.h"
#include "substrate/replace_order.h"

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

namespace substrate {

Side parse_side(const std::string&);
inline Requests parse_request_csv(const std::string& csv)
{
    auto tokens = split(csv);
    if(tokens[0] == "ADD") {
        ClientOrderID clordid{std::stoul(tokens[1].c_str())};
        Side side = tokens[2] == "0" ? Side::buy : Side::sell;
        Quantity qty{std::stoi(tokens[3].c_str())};
        return NewOrder{clordid,
                        Symbol::from_sv("AAPL"),
                        side,
                        qty,
                        qty,
                        qty,
                        Price::from_string(tokens[4])};
    } else if(tokens[0] == "CXL") {
        return CancelOrder{ClientOrderID{std::stoul(tokens[1].c_str())}};
    } else if(tokens[0] == "RPL") {
        return ReplaceOrder{};
    }

    throw std::runtime_error{std::format("Unknown request {}", tokens[0])};
}

} // namespace substrate