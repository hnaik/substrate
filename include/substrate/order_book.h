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

#include "messages.h"
#include "order.h"

#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace substrate {

template <typename Key, typename Compare>
class OrderBook {
    using TimePriorityQueue = std::list<std::unique_ptr<Order>>;
    using tqp_iterator = typename TimePriorityQueue::iterator;
    using Registry = std::unordered_map<ClientOrderID, tqp_iterator>;

public:
    using key_type = Key;

    OrderBook(const std::string& name) : name_{name} {}

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
        registry_.erase((*begin(itr->second))->orderid);
        itr->second.pop_front();
        if(itr->second.size() == 0) {
            q_.erase(itr->first);
        }
    }

    void add(const key_type& price, ClientOrderID orderid, Quantity quantity)
    {
        if(!q_.contains(price)) {
            q_.emplace(price, TimePriorityQueue());
        }
        registry_[orderid] = q_.at(price).emplace(
            end(q_.at(price)), new Order{orderid, quantity, price});
    }

    size_t size() const { return q_.size(); }
    bool empty() const { return size() == 0; }

    bool has_order(ClientOrderID orderid) const
    {
        return registry_.contains(orderid);
    }

    void remove_order(ClientOrderID orderid)
    {
        auto itr = registry_.find(orderid);
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
        registry_.erase(orderid);
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