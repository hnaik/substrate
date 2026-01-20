#pragma once

#include "substrate/sbs_protocol/TradeID.h"
#include "substrate/wrapped_type.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace substrate {
class TradeID : public WrappedType<TradeID, sbs_protocol::TradeID> {
public:
    explicit TradeID(uint64_t tradeid) { u_.value(tradeid); }
    explicit TradeID(std::string_view sv) : TradeID{std::stoull(sv.data())} {}
    explicit TradeID(const std::string& s) : TradeID{std::stoull(s.c_str())} {}

    uint64_t value() const { return u_.value(); }
};
} // namespace substrate