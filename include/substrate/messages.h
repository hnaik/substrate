#pragma once

#include <array>
#include <cstdint>

namespace substrate {
enum class RequestType : uint16_t
{
    // order entry
    new_order,
    cancel_order,
    replace_order,
    mass_cancel,
    trading_state,
    //
    engine_clock_ticks_ts
};

enum class ResponseType : uint16_t
{
    // Order lifecycle
    accepted,
    rejected,
    canceled,
    replaced,

    // Executions
    execution,

    // Market data deltas
    book_delta,
    trade_print
};

using ClOrdID = uint64_t;
// using Symbol = std::array<char, 8>;
// using Account = std::array<char, 16>;
using Timestamp = uint64_t;
using Quantity = int32_t;
using Price = double;

enum class SelfTradePolicy : uint8_t
{
    cancel_new,
    cancel_old,
    decrement,
    optional,
    //
    default_policy = cancel_new
};

enum class Side : uint8_t
{
    buy = 1,
    sell = 2,
    short_sell = 5
};

enum class OrderType : uint16_t
{
    limit = 1,
    ioc = 2,
    fok = 3,
    gtc = 4
};

enum class TIF : uint8_t
{
    day = 1,
    ioc = 2,
    fok = 3,
    gtc = 4
};

#pragma pack(push, 1)
struct NewOrder {
    ClOrdID clordid;
    char symbol[8];
    Side side;
    OrderType type;
    Quantity qty;
    Quantity min_qty;
    Quantity display_qty;
    Price price;
    TIF tif;
    char account[16];
    uint16_t flags; // future (bitset: post-only, reduce-only etc)
    Timestamp client_ts;
    SelfTradePolicy self_trade_policy;
};
#pragma pack(pop)

#
} // namespace substrate