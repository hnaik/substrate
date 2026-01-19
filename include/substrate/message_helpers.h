#pragma once

#include "messages.h"
#include <ostream>

namespace substrate {
inline std::ostream& operator<<(std::ostream& os, const NewOrder& order)
{
    os << order.u_;
    return os;
}
} // namespace substrate