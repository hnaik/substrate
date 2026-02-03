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

#include "common_types.h"
#include "string_helpers.h"

#include <format>
#include <substrate/sbs_protocol/Side.h>

#include <ostream>
#include <string>
#include <tuple>

namespace substrate {
struct Order {
    ClOrdID clordid;
    Side side;
    Quantity qty;
    Price price;

    inline static Order from_csv(const std::string& s)
    {
        auto t = substrate::split(s);
        substrate::Side side =
            t[1] == "B" ? substrate::Side::buy : substrate::Side::sell;

        return substrate::Order{substrate::ClOrdID{std::stoul(t[0].c_str())},
                                side,
                                substrate::Quantity{std::stoi(t[2].c_str())},
                                substrate::Price{t[3]}};
    }

    std::string to_string() const
    {
        return std::format("Order(ClOrdID={}|Side={}|Qty={}|Px={})",
                           clordid,
                           side::to_string(this->side),
                           qty,
                           price.display_value());
    }
};

inline bool operator==(const Order& o1, const Order& o2)
{
    return std::tie(o1.clordid, o1.side, o1.qty, o1.price) ==
           std::tie(o2.clordid, o2.side, o2.qty, o2.price);
}

inline bool operator!=(const Order& o1, const Order& o2) { return !(o1 == o2); }

inline std::ostream& operator<<(std::ostream& os, const Order& order)
{
    os << "Order(" << order.clordid << ";" << side::to_string(order.side) << ";"
       << order.qty << ";" << order.price.display_value() << ")";
    return os;
}
} // namespace substrate