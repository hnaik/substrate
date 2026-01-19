#pragma once

#include <substrate/sbs_protocol/Account.h>
#include <substrate/sbs_protocol/NewOrder.h>
#include <substrate/sbs_protocol/SelfTradePolicy.h>
#include <substrate/sbs_protocol/Side.h>
#include <substrate/sbs_protocol/Symbol.h>
#include <substrate/sbs_protocol/TIF.h>
#include <substrate/sbs_protocol/Timestamp.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace substrate {
using ClientOrderID = uint64_t;
using Quantity = int32_t;
using Price = double;

enum class Side : uint8_t
{
    buy = sbs_protocol::Side::Buy,
    sell = sbs_protocol::Side::Sell
};

enum class TIF : uint8_t
{
    day = sbs_protocol::TIF::Day,
    ioc = sbs_protocol::TIF::IOC,
    fok = sbs_protocol::TIF::FOK,
    gtc = sbs_protocol::TIF::GTC
};

enum class SelfTradePolicy : uint8_t
{
    cancel_new = sbs_protocol::SelfTradePolicy::CancelNew,
    cancel_old = sbs_protocol::SelfTradePolicy::CancelOld,
    decrement = sbs_protocol::SelfTradePolicy::Decrement,
    optional = sbs_protocol::SelfTradePolicy::Optional
};

template <typename WrappedType, typename UnderlyingType>
class TypeWrapper {
public:
    TypeWrapper() : u_{buf_begin(), size} {}

protected:
    using underlying_type = UnderlyingType;
    using wrapped_type = WrappedType;
    static constexpr std::size_t size = sizeof(underlying_type);
    using wrapped_ptr = std::unique_ptr<wrapped_type>;

    void init() { u_.wrapForEncode(buf_.data(), 0, size); }
    underlying_type& self() { return u_; }
    const underlying_type& self() const { return u_; }

    char* buf_begin()
    {
        std::fill(begin(buf_), end(buf_), 0);
        return buf_.data();
    }

    underlying_type u_;
    std::array<char, size> buf_;
};

struct Timestamp : public TypeWrapper<Timestamp, sbs_protocol::Timestamp> {
    using base_type = TypeWrapper<Timestamp, sbs_protocol::Timestamp>;
    Timestamp() : base_type{} {}
    Timestamp(uint64_t time) : Timestamp{} { u_.time(time); }
    uint64_t time() const { return u_.time(); }
};

// template <typename T>
// concept WrappedStringBase =
//     (std::same_as<T, std::string> || std::same_as<T, std::string_view>);

template <typename U>
struct WrappedString : TypeWrapper<WrappedString<U>, U> {
    using base_type = TypeWrapper<WrappedString<U>, U>;

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

using Symbol = WrappedString<sbs_protocol::Symbol>;
using Account = WrappedString<sbs_protocol::Account>;

class NewOrder : public TypeWrapper<NewOrder, sbs_protocol::NewOrder> {
    using base = TypeWrapper<NewOrder, sbs_protocol::NewOrder>;

public:
    NewOrder() : base{} {}
    NewOrder(ClientOrderID clordid,
             const Symbol& symbol,
             Side side,
             Quantity qty,
             Quantity min_qty,
             Quantity display_qty,
             Price price,
             TIF tif,
             const Account& account,
             const Timestamp& client_ts,
             SelfTradePolicy stp)
    {
        init();
        u_.clordid(clordid)
            .side(static_cast<sbs_protocol::Side::Value>(side))
            .qty(qty)
            .min_qty(min_qty)
            .display_qty(display_qty)
            .price(price)
            .tif(static_cast<sbs_protocol::TIF::Value>(tif))
            .self_trade_policy(
                static_cast<sbs_protocol::SelfTradePolicy::Value>(stp));
        u_.symbol().putData(symbol.sv());
        u_.account().putData(account.sv());
        u_.client_ts().time(client_ts.time());
    }
    ClientOrderID clordid() const { return u_.clordid(); }
    Side side() const { return static_cast<Side>(u_.side()); }
    std::string symbol() { return u_.symbol().getDataAsString(); }
    Quantity qty() const { return u_.qty(); }
    Quantity min_qty() const { return u_.min_qty(); }
    Quantity display_qty() const { return u_.display_qty(); }
    Price price() const { return u_.price(); }
    TIF tif() const { return static_cast<TIF>(u_.tif()); }
    std::string account() { return u_.account().getDataAsString(); }
    Timestamp client_ts() { return u_.client_ts().time(); }
    SelfTradePolicy stp() const
    {
        return static_cast<SelfTradePolicy>(self().self_trade_policy());
    }
};

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

} // namespace substrate