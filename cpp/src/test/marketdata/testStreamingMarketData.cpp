#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <StreamingMarketData.h>

TEST(streaming_marketdata_test_case, Spot)
{                
    using namespace std::chrono_literals;

    try {
        StreamingMarketData stream;
        stream.SecID("BTC/USD");
        stream.start();
        std::this_thread::sleep_for(1s);
        stream.stop();
        Tick t = stream.getTick("BTC/USD");
        std::cout << t << "\n";
        EXPECT_EQ(1, 1);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << "\n";
    }

}

