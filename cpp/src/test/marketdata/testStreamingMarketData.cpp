#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <StreamingMarketData.h>

TEST(streaming_marketdata_test_case, Spot)
{                
    using namespace std::chrono_literals;

    try {         
        StreamingMarketData::getInstance().SecID("BTC/USD");
        StreamingMarketData::getInstance().start();
        std::this_thread::sleep_for(1s);
        Tick t = StreamingMarketData::getInstance().getTick("BTC/USD");
        std::cout << t << "\n";
        StreamingMarketData::getInstance().stop();        
        EXPECT_EQ(1, 1);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << "\n";
    }

}

