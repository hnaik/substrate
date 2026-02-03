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

#include "logging.h"           // IWYU pragma: export
#include "substrate/version.h" // IWYU pragma: export

#include <string>
#include <string_view>

namespace substrate {
template <typename T>
class Application {
public:
    Application(std::string_view name,
                std::string_view log_format = default_log_format)
        : name_{name}, log_format_{log_format}
    {
        set_log_format(log_format_);
    }

    void run()
    {
        INFO("================================================================="
             "===============");
        INFO("#                           Starting {}", name_);
        INFO("#                           Git commit ID: {}", GIT_COMMIT_ID);
        INFO("================================================================="
             "===============");
    }

    Application& set_log_format(std::string_view log_format)
    {
        log_format_ = log_format;
        spdlog::set_pattern(log_format_);
        return *this;
    }

private:
    static constexpr const std::string_view default_log_format =
        "%Y-%m-%d %H:%M:%S.%F %L [%18s:%-3#] %v";

    std::string name_;
    std::string log_format_;
};
} // namespace substrate