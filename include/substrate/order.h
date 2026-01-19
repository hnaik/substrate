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

#include <tuple>

namespace substrate {
struct Order {
    ClientOrderID clordid;
    Quantity qty;
    Price price;
};

inline bool operator==(const Order& o1, const Order& o2)
{
    return std::tie(o1.clordid, o1.qty, o1.price) ==
           std::tie(o2.clordid, o2.qty, o2.price);
}

inline bool operator!=(const Order& o1, const Order& o2)
{
    return std::tie(o1.clordid, o1.qty, o1.price) !=
           std::tie(o2.clordid, o2.qty, o2.price);
}
} // namespace substrate