#pragma once

#include "substrate/sbs_protocol/TradeID.h"
#include "substrate/wrapped_type.h"

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

namespace substrate {
class TradeID : public WrappedType<TradeID, sbs_protocol::TradeID> {
public:
    explicit TradeID(uint64_t tradeid) { u_.value(tradeid); }
    explicit TradeID(std::string_view sv) : TradeID{std::stoull(sv.data())} {}
    explicit TradeID(const std::string& s) : TradeID{std::stoull(s.c_str())} {}
    explicit TradeID(sbs_protocol::TradeID& tradeid) { u_ = tradeid; }

    uint64_t value() const { return u_.value(); }

    inline std::string to_string() const
    {
        return std::format("TradeID({})", value());
    }
};
// using TradeID = WrappedValue<sbs_protocol::TradeID>;
} // namespace substrate