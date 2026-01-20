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
#include "protocol.h"
#include "timestamp.h"
#include "wrapped_type.h"

#include <string>
#include <substrate/sbs_protocol/NewOrder.h>
#include <substrate/sbs_protocol/Price.h>

namespace substrate {

class NewOrder : public WrappedType<NewOrder, sbs_protocol::NewOrder> {
    using base = WrappedType<NewOrder, sbs_protocol::NewOrder>;

public:
    NewOrder() = default;

    NewOrder(ClientOrderID clordid,
             const Symbol& symbol,
             Side side,
             Quantity qty,
             Quantity min_qty,
             Quantity display_qty,
             const Price& price,
             TIF tif,
             const Account& account,
             const Timestamp& client_ts,
             SelfTradePolicy stp)
        : NewOrder{}
    {
        u_.clordid(clordid)
            .side(static_cast<sbs_protocol::Side::Value>(side))
            .qty(qty)
            .min_qty(min_qty)
            .display_qty(display_qty)
            .tif(static_cast<sbs_protocol::TIF::Value>(tif))
            .self_trade_policy(
                static_cast<sbs_protocol::SelfTradePolicy::Value>(stp));
        u_.symbol().putData(symbol.sv());
        u_.account().putData(account.sv());
        u_.client_ts().time(client_ts.time());
        u_.price().value(price.value());
    }
    ClientOrderID clordid() const { return u_.clordid(); }
    Side side() const { return static_cast<Side>(u_.side()); }
    std::string symbol() const
    {
        return const_cast<underlying_type&>(u_).symbol().getDataAsString();
    }
    Quantity qty() const { return u_.qty(); }
    Quantity min_qty() const { return u_.min_qty(); }
    Quantity display_qty() const { return u_.display_qty(); }
    Price price() const
    {
        return Price::from_underlying(const_cast<underlying_type&>(u_).price());
    }
    TIF tif() const { return static_cast<TIF>(u_.tif()); }
    std::string account() const
    {
        return const_cast<underlying_type&>(u_).account().getDataAsString();
    }
    Timestamp client_ts() const
    {
        return const_cast<underlying_type&>(u_).client_ts().time();
    }
    SelfTradePolicy stp() const
    {
        return static_cast<SelfTradePolicy>(u_.self_trade_policy());
    }

private:
    underlying_type& u() { return u_; }
};
} // namespace substrate