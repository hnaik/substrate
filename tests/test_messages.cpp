#include <substrate/messages.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("NewOrder")
{
    using namespace substrate;

    SECTION("Constructor")
    {
        NewOrder order{12345,
                       Symbol::from_sv("AAPL"),
                       Side::buy,
                       Quantity{100},
                       Quantity{100},
                       Quantity{100},
                       Price::from_sv("123.45"),
                       TIF::day,
                       Account::from_sv("1000123"),
                       Timestamp{123456},
                       SelfTradePolicy::cancel_new};
        REQUIRE(order.clordid() == 12345);
        REQUIRE(order.symbol() == "AAPL");
        REQUIRE(order.side() == Side::buy);
        REQUIRE(order.qty() == Quantity{100});
        REQUIRE(order.min_qty() == Quantity{100});
        REQUIRE(order.display_qty() == Quantity{100});
        REQUIRE(order.price() == Price::from_string("123.45"));
        REQUIRE(order.tif() == TIF::day);
        REQUIRE(order.account() == "1000123");
    }
}