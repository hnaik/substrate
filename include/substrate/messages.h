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

#include "cancel_order.h" // IWYU pragma: export
#include "common_types.h"
#include "new_order.h"
#include "protocol.h"
#include "replace_order.h"
#include "wrapped_type.h"

#include <cstdint>
#include <variant>

namespace substrate {

enum class RequestType : uint16_t
{
    // order entry
    new_order,
    cancel_order,
    replace_order,
    mass_cancel,
    trading_state,
    //
    engine_clock_ticks_ts
};

enum class ResponseType : uint16_t
{
    // Order lifecycle
    accepted,
    rejected,
    canceled,
    replaced,

    // Executions
    execution,

    // Market data deltas
    book_delta,
    trade_print
};

using Requests = std::variant<NewOrder, CancelOrder, ReplaceOrder>;

class Accepted : public WrappedType<Accepted, sbs_protocol::Accepted> {
    using base_type = WrappedType<Accepted, sbs_protocol::Accepted>;

public:
    Accepted(ClientOrderID clordid) { u_.clordid(clordid); }
};

class Rejected : public WrappedType<Rejected, sbs_protocol::Rejected> {
    using base_type = WrappedType<Rejected, sbs_protocol::Rejected>;

public:
    Rejected(ClientOrderID clordid) { u_.clordid(clordid); }
};

class Canceled : public WrappedType<Canceled, sbs_protocol::Canceled> {
    using base_type = WrappedType<Canceled, sbs_protocol::Canceled>;

public:
    Canceled(ClientOrderID clordid) { u_.clordid(clordid); }
};

class Replaced : public WrappedType<Replaced, sbs_protocol::Replaced> {
    using base_type = WrappedType<Replaced, sbs_protocol::Replaced>;

public:
    Replaced(ClientOrderID clordid) { u_.clordid(clordid); }
};

class Execution : public WrappedType<Execution, sbs_protocol::Execution> {
    using base_type = WrappedType<Execution, sbs_protocol::Execution>;

public:
    Execution(ClientOrderID clordid) { u_.clordid(clordid); }
};

using Responses =
    std::variant<Accepted, Rejected, Canceled, Replaced, Execution>;

} // namespace substrate