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
#include "protocol.h"
#include "timestamp.h"
#include "utils.h"
#include "wrapped_type.h"

namespace substrate {

class ReplaceOrder
    : public WrappedType<ReplaceOrder, sbs_protocol::ReplaceOrder> {
    using base_type = WrappedType<ReplaceOrder, sbs_protocol::ReplaceOrder>;

public:
    ReplaceOrder() = default;
    ReplaceOrder(ClOrdID orig_clordid,
                 ClOrdID clordid,
                 Quantity new_qty,
                 Price new_price,
                 const Timestamp& client_ts = now_ns())
    {
        u_.orig_clordid(orig_clordid).clordid(clordid).new_qty(new_qty);
        u_.client_ts().time(client_ts.time());
        u_.new_price().value(new_price.value());
    }
};
} // namespace substrate