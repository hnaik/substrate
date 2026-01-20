#include <substrate/limit_order_book.h>

#include "substrate/logging.h"
#include "substrate/responses/execution.h"
#include <substrate/application.h>
#include <substrate/common_types.h>
#include <substrate/messages.h>
#include <substrate/new_order.h>
#include <substrate/order.h>

#include <catch2/catch_test_macros.hpp>

#include <deque>

TEST_CASE("MatchingEngine", "[basic]")
{
    substrate::Application<int> app{""};

    using namespace substrate;
    // MockLogStream logger;
    // LimitOrderBook<Price, MockLogStream> lob{logger};
    using ExecutionQueue = std::deque<responses::Execution>;
    ExecutionQueue eq;
    LimitOrderBook<Price, ExecutionQueue> lob{Symbol{"AAPL"}, eq};

    SECTION("# price levels")
    {
        REQUIRE(lob.num_price_levels() == 0);
        REQUIRE(lob.num_buy_levels() == 0);
        REQUIRE(lob.num_sell_levels() == 0);
        REQUIRE(lob.num_price_levels() == 0);

        lob.handle_add(Order::from_csv("100001,S,1,1075"));
        REQUIRE(lob.num_buy_levels() == 0);
        REQUIRE(lob.num_sell_levels() == 1);
        REQUIRE(lob.num_price_levels() == 1);

        lob.handle_add(Order::from_csv("100002,B,1,1025"));
        REQUIRE(lob.num_buy_levels() == 1);
        REQUIRE(lob.num_sell_levels() == 1);
        REQUIRE(lob.num_price_levels() == 2);

        lob.handle_add(Order::from_csv("100003,B,1,1030"));
        REQUIRE(lob.num_buy_levels() == 2);
        REQUIRE(lob.num_sell_levels() == 1);
        REQUIRE(lob.num_price_levels() == 3);
    }

    SECTION("matches")
    {
        // const auto& info_msgs = logger.info_logs();
        lob.handle_add(Order::from_csv("1000001,B,3,1050"));
        lob.handle_add(Order::from_csv("1000002,B,3,1055"));
        lob.handle_add(Order::from_csv("1000003,B,3,1045"));
        // REQUIRE(info_msgs.empty());
        lob.handle_add(Order::from_csv("1000004,S,10,1025"));

        while(!eq.empty()) {
            SPDLOG_INFO("{}", eq.front().to_string());
            eq.pop_front();
        }
    }

    SECTION("cancels") {}
}