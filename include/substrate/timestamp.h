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

#include "protocol.h"
#include "wrapped_type.h"

#include <cstdint>

namespace substrate {
struct Timestamp : public WrappedType<Timestamp, sbs_protocol::Timestamp> {
    using base_type = WrappedType<Timestamp, sbs_protocol::Timestamp>;
    Timestamp() : base_type{} {}
    Timestamp(uint64_t time) : Timestamp{} { u_.time(time); }
    uint64_t time() const { return u_.time(); }
};
} // namespace substrate