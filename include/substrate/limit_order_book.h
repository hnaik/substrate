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
#include "order.h"
#include "order_queue.h"

#include <cstddef>
#include <functional>
#include <string>

namespace substrate {
template <typename PriceKey, typename LogStream>
class LimitOrderBook {
public:
    LimitOrderBook(LogStream& logger) : logger_{logger} {}

    void handle_add(const Order& order)
    {
        if(order.side == Side::buy) {
            bids_.add(order);
        } else {
            offers_.add(order);
        }
        resolve();
    }

    void handle_cancel(const auto& req)
    {
        if(bids_.has_order(req.orderid)) {
            bids_.remove_order(req.orderid);
        } else if(offers_.has_order(req.orderid)) {
            offers_.remove_order(req.orderid);
        } else {
            logger_.err() << "CANCEL_REJECT: " << to_string(req.orderid)
                          << ", no such order to cancel" << std::endl;
        }
    }

    void handle_replace(const auto&) {}

    size_t num_buy_levels() const { return bids_.num_levels(); }
    size_t num_sell_levels() const { return offers_.num_levels(); }
    size_t num_price_levels() const
    {
        return bids_.num_levels() + offers_.num_levels();
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

    void publish_trade(Order*, Order*, const Price&)
    {
        // logger_.info() << make_trade_event(o1->qty, price) << std::endl;
        // if(o1->orderid < o2->orderid) {
        //     publish_fill(o2, o1->qty);
        //     logger_.info() << make_fill(o1->orderid) << std::endl;
        // } else {
        //     logger_.info() << make_fill(o1->orderid) << std::endl;
        //     publish_fill(o2, o1->qty);
        // }
    }

    void publish_fill(Order*, Quantity)
    {
        // if(qty == order->qty) {
        //     logger_.info() << make_fill(order->clordid) << std::endl;
        // } else {
        //     logger_.info() << make_partial(order->clordid, order->qty - qty)
        //                    << std::endl;
        // }
    }

    LogStream& logger_;
    OrderQueue<Price, std::greater<Price>> bids_{"bids"};
    OrderQueue<Price, std::less<Price>> offers_{"offers"};
};
} // namespace substrate