/* ==============================================================================
# Substrate: Financial Execution Research Platform
# ==============================================================================
# Copyright (C) 2026  Harish Naik <harishgnaik@gmail.com>

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

#include "substrate/sbs_protocol/TradeID.h"
#include "substrate/wrapped_type.h"

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

namespace substrate {
class TradeID : public WrappedType<TradeID, sbs_protocol::TradeID> {
public:
    explicit TradeID(uint64_t tradeid) { u_.value(tradeid); }
    explicit TradeID(std::string_view sv) : TradeID{std::stoull(sv.data())} {}
    explicit TradeID(const std::string& s) : TradeID{std::stoull(s.c_str())} {}
    explicit TradeID(sbs_protocol::TradeID& tradeid) { u_ = tradeid; }

    uint64_t value() const { return u_.value(); }

    inline std::string to_string() const
    {
        return std::format("TradeID({})", value());
    }
};
// using TradeID = WrappedValue<sbs_protocol::TradeID>;
} // namespace substrate