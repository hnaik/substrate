
#include <substrate/common_types.h>
#include <substrate/messages.h>
#include <substrate/order.h>
#include <substrate/order_book.h>
#include <substrate/price.h>

#include <catch2/catch_test_macros.hpp>

#include <functional>

TEST_CASE("OrderBook")
{
    using namespace substrate;

    using MinBook = OrderBook<substrate::Price, std::less<Price>>;
    using MaxBook = OrderBook<substrate::Price, std::greater<Price>>;

    SECTION("Buy")
    {
        MaxBook buys{"Buy"};

        SECTION("Size")
        {
            REQUIRE(buys.size() == 0);

            buys.add(Price{"12345"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(buys.size() == 1);

            buys.add(Price{"12350"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(buys.size() == 2);

            buys.add(Price{"12350"}, ClientOrderID{12345}, Quantity{2000});
            REQUIRE(buys.size() == 2);
        }

        SECTION("top")
        {
            REQUIRE(buys.top() == nullptr);

            // Initial order
            buys.add(Price{"12345"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(buys.top()->clordid == ClientOrderID{12345});
            REQUIRE(buys.top()->qty == ClientOrderID{1000});

            // Lower buy order
            buys.add(Price{"12340"}, ClientOrderID{23456}, Quantity{1500});
            REQUIRE(buys.top()->clordid == ClientOrderID{12345});
            REQUIRE(buys.top()->qty == ClientOrderID{1000});

            // Higher buy order
            buys.add(Price{"12350"}, ClientOrderID{43456}, Quantity{1500});
            REQUIRE(buys.top()->clordid == ClientOrderID{43456});
            REQUIRE(buys.top()->qty == ClientOrderID{1500});

            // New high order, same as current top, lower time priority
            buys.add(Price{"12350"}, ClientOrderID{34567}, Quantity{2500});
            REQUIRE(buys.top()->clordid == ClientOrderID{43456});
            REQUIRE(buys.top()->qty == ClientOrderID{1500});
        }

        SECTION("pop")
        {
            REQUIRE(buys.empty());

            Order null_order{0, 0, Price{"0"}};

            // Initial order
            buys.add(Price{"12345"}, ClientOrderID{12345}, Quantity{1000});
            auto order_1 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_1 == Order{ClientOrderID{12345},
                                     Quantity{1000},
                                     Price{"12345"}});

            buys.pop();
            REQUIRE(buys.empty());
            REQUIRE(buys.top() == nullptr);

            buys.add(Price{"12340"}, ClientOrderID{23456}, Quantity{1500});
            buys.add(Price{"12350"}, ClientOrderID{43456}, Quantity{1500});
            buys.add(Price{"12350"}, ClientOrderID{34567}, Quantity{2500});

            auto order_2 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_2 == Order{ClientOrderID{43456},
                                     Quantity{1500},
                                     Price{"12350"}});

            buys.pop();

            REQUIRE(buys.top());
            auto order_3 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_3 == Order{ClientOrderID{34567},
                                     Quantity{2500},
                                     Price{"12350"}});

            buys.pop();
            buys.pop();

            REQUIRE(buys.empty());
        }
    }

    SECTION("Sell")
    {
        MinBook sells{"Sell"};

        SECTION("Size")
        {
            REQUIRE(sells.size() == 0);

            sells.add(Price{"12345"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(sells.size() == 1);

            sells.add(Price{"12350"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(sells.size() == 2);

            sells.add(Price{"12350"}, ClientOrderID{12345}, Quantity{2000});
            REQUIRE(sells.size() == 2);
        }

        SECTION("top")
        {
            REQUIRE(sells.top() == nullptr);

            // Initial order
            sells.add(Price{"12340"}, ClientOrderID{12345}, Quantity{1000});
            REQUIRE(sells.top()->clordid == ClientOrderID{12345});
            REQUIRE(sells.top()->qty == ClientOrderID{1000});

            // Higher sell order
            sells.add(Price{"12345"}, ClientOrderID{23456}, Quantity{1500});
            REQUIRE(sells.top()->clordid == ClientOrderID{12345});
            REQUIRE(sells.top()->qty == ClientOrderID{1000});

            // Lower sell order
            sells.add(Price{"12325"}, ClientOrderID{23456}, Quantity{1500});
            REQUIRE(sells.top()->clordid == ClientOrderID{23456});
            REQUIRE(sells.top()->qty == ClientOrderID{1500});

            // New lower order, same as top price, lower time priority
            sells.add(Price{"12325"}, ClientOrderID{34567}, Quantity{2500});
            REQUIRE(sells.top()->clordid == ClientOrderID{23456});
            REQUIRE(sells.top()->qty == ClientOrderID{1500});
        }

        SECTION("pop")
        {
            REQUIRE(sells.empty());

            Order null_order{0, 0, Price{"0"}};

            // Initial order
            sells.add(Price{"12345"}, ClientOrderID{12345}, Quantity{1000});

            auto order_1 = sells.top() ? *sells.top() : null_order;
            REQUIRE(order_1 == Order{ClientOrderID{12345},
                                     Quantity{1000},
                                     Price{"12345"}});
            sells.pop();
            REQUIRE(sells.empty());
            REQUIRE(sells.top() == nullptr);

            sells.add(Price{"12350"}, ClientOrderID{23456}, Quantity{1500});
            sells.add(Price{"12325"}, ClientOrderID{23456}, Quantity{1500});
            sells.add(Price{"12325"}, ClientOrderID{34567}, Quantity{2500});

            auto order_2 = sells.top() ? *sells.top() : null_order;
            REQUIRE(order_2 == Order{ClientOrderID{23456},
                                     Quantity{1500},
                                     Price{"12325"}});

            sells.pop();
            auto order_3 = sells.top() ? *sells.top() : null_order;
            REQUIRE(order_3 == Order{ClientOrderID{34567},
                                     Quantity{2500},
                                     Price{"12325"}});

            sells.pop();
            sells.pop();

            REQUIRE(sells.empty());
        }
    }
}