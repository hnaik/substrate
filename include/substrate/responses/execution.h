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

#include "substrate/common_types.h"
#include "substrate/wrapped_type.h"
#include "trade_id.h"

#include "substrate/sbs_protocol/Execution.h"

#include <format>
#include <json/json.h>

#include <ostream>
#include <string>

namespace substrate::responses {

class Execution
    : public substrate::WrappedType<Execution, sbs_protocol::Execution> {
public:
    Execution() = default;
    Execution(ClOrdID clordid,
              const TradeID& tradeid,
              const Symbol& symbol,
              const Price& price,
              Quantity qty,
              Quantity leaves_qty)
    {
        u_.clordid(clordid).qty(qty).leaves_qty(leaves_qty);
        u_.tradeid().value(tradeid.value());
        u_.symbol().putData(symbol.sv());
        u_.price().value(price.value());
    }

    ClOrdID clordid() const { return u_.clordid(); }

    TradeID tradeid() const
    {
        return TradeID{const_cast<underlying_type&>(u_).tradeid()};
    }

    Symbol symbol() const
    {
        return Symbol{const_cast<underlying_type&>(u_).symbol().data()};
    }

    Price price() const
    {
        return Price{const_cast<underlying_type&>(u_).price()};
    }

    inline std::string to_string() const
    {
        return std::format(
            "Execution(Sym={}|ClOrdID={}|TradeID={}|Price={:.2f}|"
            "Qty={}|LeavesQty={})",
            symbol().str(),
            u_.clordid(),
            tradeid().value(),
            price().display_value(),
            u_.qty(),
            u_.leaves_qty());
    }

    inline std::ostream& operator<<(std::ostream& os)
    {
        os << u_;
        return os;
    }
};

} // namespace substrate::responses
