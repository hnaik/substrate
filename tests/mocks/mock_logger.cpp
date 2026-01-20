#include "mock_logger.h"

namespace substrate {
DefaultMockLogger& MockLogStream::info() { return info_; }
DefaultMockLogger& MockLogStream::err() { return error_; }
const DefaultMockLogger::log_container& MockLogStream::info_logs() const
{
    return info_.messages();
}

const DefaultMockLogger::log_container& MockLogStream::err_logs() const
{
    return error_.messages();
}
} // namespace substrate