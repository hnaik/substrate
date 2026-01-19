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

#include <substrate/message_helpers.h>
#include <substrate/messages.h>

#include <iostream>

int main()
{

    using namespace substrate;
    NewOrder order{12345,
                   Symbol::from_sv("AAPL"),
                   Side::buy,
                   Quantity{100},
                   Quantity{100},
                   Quantity{100},
                   Price{123.45},
                   TIF::day,
                   Account::from_sv("1000123"),
                   Timestamp{123456},
                   SelfTradePolicy::cancel_new};
    std::cout << "hello, substrate world!" << std::endl;
    std::cout << order << std::endl;
}
