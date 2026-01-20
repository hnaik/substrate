
#include <string_helpers.h>
#include <substrate/application.h>
#include <substrate/common_types.h>
#include <substrate/messages.h>
#include <substrate/order.h>
#include <substrate/order_queue.h>
#include <substrate/price.h>

#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <string>

substrate::Order make_order(const std::string& s)
{
    auto t = substrate::split(s);
    substrate::Side side =
        t[1] == "B" ? substrate::Side::buy : substrate::Side::sell;

    return substrate::Order{substrate::ClientOrderID{std::stoul(t[0].c_str())},
                            side,
                            substrate::Quantity{std::stoi(t[2].c_str())},
                            substrate::Price{t[3]}};
}

TEST_CASE("OrderBook")
{
    using namespace substrate;
    Application<int> app{""};

    using MaxPriorityQueue = OrderQueue<substrate::Price, std::greater<Price>>;

    SECTION("Buy")
    {
        MaxPriorityQueue buys{"Buy"};

        SECTION("Size")
        {
            REQUIRE(buys.size() == 0);

            buys.add(make_order("12344,B,1000,123450"));
            REQUIRE(buys.size() == 1);
            REQUIRE(buys.num_levels() == 1);

            buys.add(make_order("12345,B,1000,123450"));
            REQUIRE(buys.size() == 2);
            REQUIRE(buys.num_levels() == 1);

            buys.add(make_order("12345,B,2000,123450"));
            REQUIRE(buys.size() == 2);
            REQUIRE(buys.num_levels() == 1);
        }

        SECTION("top")
        {
            REQUIRE(buys.top() == nullptr);

            // Initial order
            buys.add(make_order("100001,B,1000,12345"));
            REQUIRE(buys.top()->clordid == ClientOrderID{100001});
            REQUIRE(buys.top()->price == Price{"12345"});
            REQUIRE(buys.top()->qty == Quantity{1000});

            // Lower buy order
            buys.add(make_order("100002,B,1500,12340"));
            REQUIRE(buys.top()->clordid == ClientOrderID{100001});
            REQUIRE(buys.top()->qty == Quantity{1000});

            // Higher buy order
            buys.add(make_order("100003,B,1500,12350"));
            REQUIRE(buys.top()->clordid == ClientOrderID{100003});
            REQUIRE(buys.top()->qty == Quantity{1500});

            // New high order, same as current top, lower time priority
            buys.add(make_order("100004,B,2500,12350"));
            REQUIRE(buys.top()->clordid == ClientOrderID{100003});
            REQUIRE(buys.top()->qty == Quantity{1500});
        }

        SECTION("pop")
        {
            REQUIRE(buys.empty());

            // Order null_order{0, 0, Price{"0"}};
            auto null_order = make_order("0,B,0,0");

            // Initial order
            buys.add(make_order("12345,B,1000,12345"));
            auto order_1 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_1 == Order{ClientOrderID{12345},
                                     Side::buy,
                                     Quantity{1000},
                                     Price{"12345"}});

            buys.pop();
            REQUIRE(buys.empty());
            REQUIRE(buys.top() == nullptr);

            buys.add(make_order("23456,B,1500,12340"));
            buys.add(make_order("43456,B,1500,12350"));
            buys.add(make_order("34567,B,2500,12350"));

            auto order_2 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_2 == Order{ClientOrderID{43456},
                                     Side::buy,
                                     Quantity{1500},
                                     Price{"12350"}});

            buys.pop();

            REQUIRE(buys.top());
            auto order_3 = buys.top() ? *buys.top() : null_order;
            REQUIRE(order_3 == Order{ClientOrderID{34567},
                                     Side::buy,
                                     Quantity{2500},
                                     Price{"12350"}});

            buys.pop();
            buys.pop();

            REQUIRE(buys.empty());
        }
    }

    SECTION("Sell")
    {
        using MinPriorityQueue = OrderQueue<substrate::Price, std::less<Price>>;
        MinPriorityQueue sells{"Sell"};

        SECTION("Size")
        {
            REQUIRE(sells.size() == 0);

            sells.add(make_order("100001,S,1000,12345"));
            REQUIRE(sells.size() == 1);
            REQUIRE(sells.num_levels() == 1);

            sells.add(make_order("100002,S,1000,12345"));
            REQUIRE(sells.size() == 2);
            REQUIRE(sells.num_levels() == 1);

            sells.add(make_order("100002,S,2000,12350"));
            REQUIRE(sells.size() == 2);
            REQUIRE(sells.num_levels() == 1);

            sells.add(make_order("100003,S,2000,12350"));
            REQUIRE(sells.size() == 3);
            REQUIRE(sells.num_levels() == 2);
        }

        SECTION("top")
        {
            REQUIRE(sells.top() == nullptr);

            // Initial order
            sells.add(make_order("100001,S,1000,12340"));
            REQUIRE(sells.top()->clordid == ClientOrderID{100001});
            REQUIRE(sells.top()->qty == Quantity{1000});

            // Higher sell order
            sells.add(make_order("100002,S,1500,12345"));
            REQUIRE(sells.top()->clordid == ClientOrderID{100001});
            REQUIRE(sells.top()->qty == Quantity{1000});

            // Lower sell order
            sells.add(make_order("100003,S,1500,12335"));
            REQUIRE(sells.top()->clordid == ClientOrderID{100003});
            REQUIRE(sells.top()->qty == Quantity{1500});

            // New lower order, same as top price, lower time priority
            sells.add(make_order("100004,S,2500,12335"));
            REQUIRE(sells.top()->clordid == ClientOrderID{100003});
            REQUIRE(sells.top()->qty == Quantity{1500});
        }

        //     SECTION("pop")
        //     {
        //         REQUIRE(sells.empty());

        //         Order null_order{0, Side::sell, 0, Price{"0"}};

        //         // Initial order
        //         // sells.add(Price{"12345"}, ClientOrderID{12345},
        //         Quantity{1000}); sells.add(make_order("12345,S,1000,12345"));

        //         auto order_1 = sells.top() ? *sells.top() : null_order;
        //         REQUIRE(order_1 == Order{ClientOrderID{12345},
        //                                  Side::sell,
        //                                  Quantity{1000},
        //                                  Price{"12345"}});
        //         sells.pop();
        //         REQUIRE(sells.empty());
        //         REQUIRE(sells.top() == nullptr);

        //         // sells.add(Price{"12350"}, ClientOrderID{23456},
        //         Quantity{1500}); sells.add(make_order("23456,S,1500,12350"));
        //         // sells.add(Price{"12325"}, ClientOrderID{23456},
        //         Quantity{1500}); sells.add(make_order("23456,S,1500,12345"));
        //         // sells.add(Price{"12325"}, ClientOrderID{34567},
        //         Quantity{2500}); sells.add(make_order("34567,S,2500,12345"));

        //         auto order_2 = sells.top() ? *sells.top() : null_order;
        //         REQUIRE(order_2 == Order{ClientOrderID{23456},
        //                                  Side::sell,
        //                                  Quantity{1500},
        //                                  Price{"12325"}});

        //         sells.pop();
        //         auto order_3 = sells.top() ? *sells.top() : null_order;
        //         REQUIRE(order_3 == Order{ClientOrderID{34567},
        //                                  Side::sell,
        //                                  Quantity{2500},
        //                                  Price{"12325"}});

        //         sells.pop();
        //         sells.pop();

        //         REQUIRE(sells.empty());
        //     }
    }
}