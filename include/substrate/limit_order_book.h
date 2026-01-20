/* ==============================================================================
# Substrate: Financial Execution Research Platform
# ==============================================================================
# Copyright (C) 2026  Harish Naik

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU LesserGeneral Public License along
# with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
# ============================================================================*/

/*
Reference:
https://onepagecode.substack.com/p/electronic-market-structure-and-trading
*/

#pragma once

#include "common_types.h"
#include "logging.h"
#include "order.h"
#include "order_queue.h"
#include "substrate/responses/trade_id.h"
#include "utils.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <string>
#include <utility>

namespace substrate {
template <typename PriceKey, typename ExecutionQueue>
class LimitOrderBook {
public:
    LimitOrderBook(const Symbol& symbol, ExecutionQueue& eq)
        : symbol_{symbol}, eq_{eq}
    {
    }

    void handle_add(const Order& order)
    {
        if(order.side == Side::buy) {
            bids_.add(order);
        } else {
            offers_.add(order);
        }
        resolve();
    }

    void handle_cancel(ClOrdID clordid)
    {
        if(bids_.has_order(clordid)) {
            bids_.remove_order(clordid);
        } else if(offers_.has_order(clordid)) {
            offers_.remove_order(clordid);
        } else {
            ERROR("CANCEL_REJECT: {}, no such order to cancel", clordid);
        }
    }

    void handle_replace(const auto&) {}

    size_t num_buy_levels() const { return bids_.num_levels(); }
    size_t num_sell_levels() const { return offers_.num_levels(); }
    size_t num_price_levels() const
    {
        return num_buy_levels() + num_sell_levels();
    }

private:
    void resolve()
    {
        while(offers_.best_price() && bids_.best_price() &&
              offers_.best_price() <= bids_.best_price()) {
            auto* sell = offers_.top();
            auto* buy = bids_.top();

            auto best_price = offers_.best_price().value();
            if(buy->qty <= sell->qty) {
                publish_trade(buy, sell, best_price);
                sell->qty -= buy->qty;
                bids_.pop();
                if(sell->qty == 0) {
                    offers_.pop();
                }
            } else {
                publish_trade(sell, buy, best_price);
                buy->qty -= sell->qty;
                offers_.pop();
            }
        }
    }

    void publish_trade(Order* o1, Order* o2, const Price& price)
    {

        // Quantity qty = o1->qty < o2->qty ? o1->qty : o2->qty;
        // Quantity leaves = o1->qty < o2->qty ? o2->qty - qty : o2->qty - qty;
        // responses::Execution e1{
        //     o1->clordid, TradeID{now_ns()}, symbol_, price, qty, leaves};
        // responses::Execution e2{
        //     o2->clordid, TradeID{now_ns()}, symbol_, price, qty, leaves};

        // TradeID tid{now_ns()};

        if(o1->clordid < o2->clordid) {
            INFO("TRD {} x {} FillPrice:{:.2f}",
                 o2->to_string(),
                 o1->to_string(),
                 price.display_value());
            pub_trade(o2, o1, price);
        } else {
            INFO("TRD {} x {} FillPrice:{:.2f}",
                 o1->to_string(),
                 o2->to_string(),
                 price.display_value());
            pub_trade(o1, o2, price);
        }
    }

    void pub_trade(Order* o1, Order* o2, const Price& price)
    {
        TradeID tid{now_ns()};
        Quantity qty = o1->qty < o2->qty ? o1->qty : o2->qty;
        eq_.emplace_back(o1->clordid,
                         tid,
                         symbol_,
                         price,
                         qty,
                         std::max(0, o1->qty - o2->qty));
        eq_.emplace_back(o2->clordid,
                         tid,
                         symbol_,
                         price,
                         qty,
                         std::max(0, o2->qty - o2->qty));
    }

    Symbol symbol_;
    ExecutionQueue& eq_;
    OrderQueue<Price, std::greater<Price>> bids_{"bids"};
    OrderQueue<Price, std::less<Price>> offers_{"offers"};
};
} // namespace substrate