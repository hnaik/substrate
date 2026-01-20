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

#include <concepts>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace substrate {
// template <typename T>
//     requires(!std::same_as<T, std::string>)
// std::string to_string(T&& value)
// {
//     return std::to_string(std::forward<T>(value));
// }

// template <typename T>
//     requires(!std::same_as<T, std::string>)
// std::ostream& operator<<(std::ostream& os, const T& value)
// {
//     os << value.to_string();
//     return os;
// }

std::vector<std::string> split(const std::string& s, char delim = ',');
} // namespace substrate