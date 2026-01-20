#pragma once

#include "substrate/common_types.h"
#include "substrate/wrapped_type.h"
#include "trade_id.h"

#include "substrate/sbs_protocol/Execution.h"

#include <format>
#include <json/json.h>

#include <ostream>
#include <string>

namespace substrate::responses {

class Execution
    : public substrate::WrappedType<Execution, sbs_protocol::Execution> {
public:
    Execution() = default;
    Execution(ClOrdID clordid,
              const TradeID& tradeid,
              const Symbol& symbol,
              const Price& price,
              Quantity qty,
              Quantity leaves_qty)
    {
        u_.clordid(clordid).qty(qty).leaves_qty(leaves_qty);
        u_.tradeid().value(tradeid.value());
        u_.symbol().putData(symbol.sv());
        u_.price().value(price.value());
    }

    ClOrdID clordid() const { return u_.clordid(); }

    TradeID tradeid() const
    {
        return TradeID{const_cast<underlying_type&>(u_).tradeid()};
    }

    Symbol symbol() const
    {
        return Symbol{const_cast<underlying_type&>(u_).symbol().data()};
    }

    Price price() const
    {
        return Price{const_cast<underlying_type&>(u_).price()};
    }

    inline std::string to_string() const
    {
        return std::format(
            "Execution(Sym={}|ClOrdID={}|TradeID={}|Price={:.2f}|"
            "Qty={}|LeavesQty={})",
            symbol().str(),
            u_.clordid(),
            tradeid().value(),
            price().display_value(),
            u_.qty(),
            u_.leaves_qty());
    }

    inline std::ostream& operator<<(std::ostream& os)
    {
        os << u_;
        return os;
    }
};

} // namespace substrate::responses
