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

#pragma once

#include "common_types.h"
#include "order.h"
#include "spdlog/spdlog-inl.h"
#include "substrate/application.h"
#include "substrate/logging.h"

#include <cstddef>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace substrate {

template <typename Key, typename Compare>
class OrderQueue {
    using TimePriorityQueue = std::list<std::unique_ptr<Order>>;
    using tqp_iterator = typename TimePriorityQueue::iterator;
    using Registry = std::unordered_map<ClientOrderID, tqp_iterator>;

public:
    using key_type = Key;

    OrderQueue(const std::string& name) : name_{name} {}

    [[nodiscard]] Order* top() const
    {
        if(size() > 0) {
            auto itr = begin(q_);
            if(itr->second.size() == 0) {
                throw std::runtime_error{name_ + ": queue is empty"};
            }
            return itr->second.front().get();
        }
        return nullptr;
    }

    [[nodiscard]] std::optional<Price> best_price() const
    {
        if(size() == 0)
            return std::nullopt;
        return begin(q_)->first;
    }

    void pop()
    {
        if(size() == 0) {
            return;
        }
        auto itr = begin(q_);
        if(itr->second.size() == 0) {
            return;
        }
        registry_.erase((*begin(itr->second))->clordid);
        itr->second.pop_front();
        if(itr->second.size() == 0) {
            q_.erase(itr->first);
        }
    }

    void
    add(ClientOrderID clordid, Side side, Quantity qty, const key_type& price)
    {
        if(registry_.contains(clordid)) {
            WARN("ClOrdID {} exists, not adding", clordid);
            return;
        }

        if(!q_.contains(price)) {
            q_.emplace(price, TimePriorityQueue());
        }

        INFO("Adding ClOrdID {}", clordid);
        auto itr = q_.find(price);
        registry_[clordid] = itr->second.emplace(
            end(itr->second), new Order{clordid, side, qty, price});
    }

    void add(const Order& order)
    {
        add(order.clordid, order.side, order.qty, order.price);
    }

    size_t num_levels() const { return q_.size(); }
    size_t size() const
    {
        return std::accumulate(
            begin(q_),
            end(q_),
            static_cast<size_t>(0),
            [](size_t p, const auto& tq) { return p + tq.second.size(); });
    }
    bool empty() const { return size() == 0; }

    bool has_order(ClientOrderID clordid) const
    {
        return registry_.contains(clordid);
    }

    void remove_order(ClientOrderID clordid)
    {
        auto itr = registry_.find(clordid);
        if(itr == end(registry_)) {
            // Optionally print an error if the spec states this requirement
            return;
        }

        auto p_itr = q_.find((*itr->second)->price);
        if(p_itr == end(q_)) {
            // This is a more serious error since this means that data is
            // found in one lookup and not the other
            return;
        }
        p_itr->second.erase(itr->second);
        registry_.erase(clordid);
        if(p_itr->second.empty()) {
            q_.erase(p_itr->first);
        }
    }

private:
    const std::string name_;
    std::map<key_type, TimePriorityQueue, Compare> q_;
    Registry registry_;
};
} // namespace substrate