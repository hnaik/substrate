#pragma once

#include "string_helpers.h"

#include <substrate/messages.h>

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace substrate {
template <typename T>
class MockLogger {
public:
    using log_type = T;
    using log_container = std::vector<log_type>;

    MockLogger() = default;
    MockLogger(const MockLogger&) = delete;
    const MockLogger& operator=(const MockLogger&) = delete;

    MockLogger& operator<<(const std::string& s)
    {
        s_ += s;
        return *this;
    }

    template <typename U>
    MockLogger& operator<<(T&& v)
    {
        s_ += to_string(v);
        return *this;
    }

    MockLogger& operator<<(std::ostream& (*manip)(std::ostream&))
    {
        if(manip) {
        }
        m_.push_back(s_);
        s_.clear();
        return *this;
    }

    const log_container& messages() const { return m_; }

    void reset() { m_.clear(); }

private:
    log_type s_;
    log_container m_;
};

template <typename T, typename U>
    requires IsResponse<T>
MockLogger<U>& operator<<(MockLogger<U>& logger, const T& value)
{
    logger << value.to_string();
    return logger;
}

using DefaultMockLogger = MockLogger<std::string>;

struct MockLogStream {
    DefaultMockLogger& info();
    DefaultMockLogger& err();

    const DefaultMockLogger::log_container& info_logs() const;
    const DefaultMockLogger::log_container& err_logs() const;

    DefaultMockLogger info_;
    DefaultMockLogger error_;
};
} // namespace substrate