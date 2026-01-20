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

#include <array>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

namespace substrate {
template <typename WrappedT,
          typename UnderlyingT = typename WrappedT::underlying_type>
class WrappedType {
public:
    using wrapped_type = WrappedT;
    using underlying_type = UnderlyingT;

    WrappedType() : u_{buf_begin(), size} {}

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << u_;
        return oss.str();
    }

    template <typename W, typename U>
    friend std::ostream& operator<<(std::ostream& os, const WrappedType<W, U>);

    static wrapped_type from_underlying(underlying_type u)
    {
        wrapped_type wt;
        wt.reset(std::move(u));
        return wt;
    }

protected:
    static constexpr std::size_t size = sizeof(underlying_type);

    void reset(underlying_type u) { u_ = std::move(u); }

    char* buf_begin()
    {
        std::fill(begin(buf_), end(buf_), 0);
        return buf_.data();
    }

    underlying_type u_;
    std::array<char, size> buf_;
};

template <typename W, typename U>
std::ostream& operator<<(std::ostream& os, const WrappedType<W, U>& wt)
{
    os << wt.u_;
    return os;
}

template <typename U>
struct WrappedString : WrappedType<WrappedString<U>, U> {
    using base_type = WrappedType<WrappedString<U>, U>;

    template <typename T>
    // requires WrappedStringBase<T>
    WrappedString(T&& s) : base_type{}
    {
        base_type::u_.putData(std::forward<T>(s));
    }

    std::string str() const { return base_type::u_.getDataAsString(); }
    std::string_view sv() const { return base_type::u_.getDataAsStringView(); }

    static WrappedString from_string(const std::string& s)
    {
        return WrappedString{s};
    }

    static WrappedString from_sv(std::string_view sv)
    {
        return WrappedString{sv};
    }
};

// template <typename U>
// class WrappedValue : WrappedType<WrappedValue<U>, U> {
// public:
//     using base_type = WrappedType<WrappedValue<U>, U>;
//     template <typename T>
//     WrappedValue(T v)
//     {
//         this->u_.value(v);
//     }

//     auto value() const { return this->u_.value(); }
//     std::string to_string() const
//     {
//         return std::format("{}({})", this->name_, value());
//     }

//     std::ostream& operator<<(std::ostream& os)
//     {
//         os << to_string();
//         return os;
//     }

// private:
//     std::string name_;
// };
} // namespace substrate