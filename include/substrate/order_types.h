#pragma once

#include <cstdint>

namespace rootmethod::rootmethod::substrate {
enum class OrderType : uint16_t
{
    // Reference: https://www.rulematch.com/trading/itch-ouch/#post-1261
    market,
    best_limit,
    market_to_limit,
    iceberg,
    imbalance
};
} // namespace rootmethod::rootmethod::substrate