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

#include "price.h"
#include "protocol.h"
#include "wrapped_type.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <substrate/sbs_protocol/Side.h>

namespace substrate {
using ClOrdID = uint64_t;
using Quantity = int32_t;

enum class Side : uint8_t
{
    buy = sbs_protocol::Side::Buy,
    sell = sbs_protocol::Side::Sell
};

enum class TIF : uint8_t
{
    day = sbs_protocol::TIF::Day,
    ioc = sbs_protocol::TIF::IOC,
    fok = sbs_protocol::TIF::FOK,
    gtc = sbs_protocol::TIF::GTC
};

enum class SelfTradePolicy : uint8_t
{
    cancel_new = sbs_protocol::SelfTradePolicy::CancelNew,
    cancel_old = sbs_protocol::SelfTradePolicy::CancelOld,
    decrement = sbs_protocol::SelfTradePolicy::Decrement,
    optional = sbs_protocol::SelfTradePolicy::Optional
};

using Price = PriceBase<double>;
using Symbol = WrappedString<sbs_protocol::Symbol>;
using Account = WrappedString<sbs_protocol::Account>;

namespace side {
inline std::string to_string(Side side)
{
    std::ostringstream oss;
    oss << static_cast<sbs_protocol::Side::Value>(side);
    return oss.str();
}
} // namespace side
} // namespace substrate