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

#include "wrapped_type.h"

#include <substrate/sbs_protocol/Price.h>

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

namespace substrate {
template <typename T, size_t Denom = 1'000>
class PriceBase : public WrappedType<PriceBase<T>, sbs_protocol::Price> {
    using base_type = WrappedType<PriceBase<T>, sbs_protocol::Price>;

public:
    using underlying_type = typename base_type::underlying_type;
    using primitive_type = int64_t;

    PriceBase() = default;
    explicit PriceBase(underlying_type price)
    {
        underlying_type::reset(std::move(price));
    }

    T display_value() const
    {
        return static_cast<T>(this->u_.value()) / static_cast<T>(Denom);
    }

    template <typename W, size_t N>
    friend std::ostream& operator<<(std::ostream& os, const PriceBase<W, N>& p);

    template <typename W, size_t N>
    bool operator==(const PriceBase<W, N>& other) const
    {
        return value() == other.value();
    }

    template <typename W, size_t N>
    bool operator!=(const PriceBase<W, N>& other) const
    {
        return value() != other.value();
    }

    static PriceBase from_sv(std::string_view sv)
    {
        PriceBase pb;
        auto v = static_cast<primitive_type>(std::stod(sv.data()) * Denom);
        pb.u_.value(v);
        return pb;
    }

    static PriceBase from_string(const std::string& s)
    {
        PriceBase pb;
        auto v = static_cast<primitive_type>(std::stod(s.c_str()) * Denom);
        pb.u_.value(v);
        return pb;
    }

    primitive_type value() const { return this->u_.value(); }
};

template <typename W, size_t N>
std::ostream& operator<<(std::ostream& os, const PriceBase<W, N>& p)
{
    os << p.display_value();
    return os;
}

} // namespace substrate