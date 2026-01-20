#pragma once

#include "substrate/common_types.h"
#include "substrate/wrapped_type.h"
#include "trade_id.h"

#include "substrate/sbs_protocol/Execution.h"

namespace substrate::responses {

class Execution
    : public substrate::WrappedType<Execution, sbs_protocol::Execution> {
public:
    Execution() = default;
    Execution(ClientOrderID clordid,
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

private:
};
} // namespace substrate::responses
