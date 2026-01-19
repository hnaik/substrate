#include <substrate/price.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("PriceBase")
{
    using namespace substrate;
    auto p1 = PriceBase<double>::from_sv("123.25");
    auto p2 = PriceBase<double>::from_sv("123.25");
    auto p3 = PriceBase<double>::from_sv("23.250");

    REQUIRE(p1 == p2);
    REQUIRE(p1 != p3);
    REQUIRE(p2 != p3);
}