#include <substrate/limit_order_book.h>

#include <substrate/common_types.h>
#include <substrate/messages.h>
#include <substrate/new_order.h>
#include <substrate/order.h>

#include "mocks/mock_logger.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("MatchingEngine", "[basic]")
{
    using namespace substrate;
    MockLogStream logger;
    LimitOrderBook<Price, MockLogStream> lob{logger};

    SECTION("# price levels")
    {
        // REQUIRE(lob.num_price_levels() == 0);
        // REQUIRE(lob.num_buy_levels() == 0);
        // REQUIRE(lob.num_sell_levels() == 0);
        // REQUIRE(lob.num_price_levels() == 0);

        // // engine.process_csv("ADD,1000000,1,1,1075");
        // lob.handle_add(Order{
        //     ClientOrderID{1000001}, Side::buy, Quantity{1}, Price{"1075"}});
        // REQUIRE(lob.num_buy_levels() == 0);
        // REQUIRE(lob.num_sell_levels() == 1);
        // REQUIRE(lob.num_price_levels() == 1);

        // engine.process_csv("0,1000000,0,1,1025");
        // REQUIRE(engine.num_buy_levels() == 1);
        // REQUIRE(engine.num_sell_levels() == 1);
        // REQUIRE(engine.num_price_levels() == 2);

        // engine.process_csv("0,1000000,0,1,1030");
        // REQUIRE(engine.num_buy_levels() == 2);
        // REQUIRE(engine.num_sell_levels() == 1);
        // REQUIRE(engine.num_price_levels() == 3);
    }

    SECTION("matches")
    {
        // const auto& info_msgs = logger.info_logs();
        // engine.process_csv("0,1000008,0,3,1050");
        // REQUIRE(info_msgs.empty());
        // engine.process_csv("0,1000007,1,5,1025");
        // REQUIRE(info_msgs[0] == "2,3,1025");
        // REQUIRE(info_msgs[1] == "3,1000008");
        // REQUIRE(info_msgs[2] == "4,1000007,2");
    }

    SECTION("cancels") {}
}