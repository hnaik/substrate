#include "utils.h"

#include <chrono>
#include <cstdint>

namespace substrate {
uint64_t now_ns()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto duration = duration_cast<nanoseconds>(now.time_since_epoch());
    return static_cast<uint64_t>(duration.count());
}
} // namespace substrate