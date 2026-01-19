#include <substrate/message_helpers.h>
#include <substrate/messages.h>

#include <iostream>

int main()
{

    using namespace substrate;
    NewOrder order{12345,
                   Symbol::from_sv("AAPL"),
                   Side::buy,
                   Quantity{100},
                   Quantity{100},
                   Quantity{100},
                   Price{123.45},
                   TIF::day,
                   Account::from_sv("1000123"),
                   Timestamp{123456},
                   SelfTradePolicy::cancel_new};
    std::cout << "hello, substrate world!" << std::endl;
    std::cout << order << std::endl;
}
