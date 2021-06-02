#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <StreamingMarketData.h>

#define GTEST_COUT std::cerr << "[          ] "

TEST(streaming_marketdata_test_case, Spot)
{                
    using namespace std::chrono_literals;

    try {         
        StreamingMarketData::getInstance().SecID("BTC/USD");
        StreamingMarketData::getInstance().start();
        std::this_thread::sleep_for(1s);
        Tick t = StreamingMarketData::getInstance().getTick("BTC/USD");
        std::cout << t << "\n"; 
        StreamingMarketData::getInstance().Subscribe("BTC-PERP");
        std::this_thread::sleep_for(1s);
        t = StreamingMarketData::getInstance().getTick("BTC-PERP");
        std::cout << t << "\n";
        StreamingMarketData::getInstance().stop();        
        ASSERT_TRUE(true);
        
    }
    catch (const std::exception& ex)
    {
        GTEST_COUT << ex.what()  << std::endl;
        FAIL();
    }
}

// Some of these the market feed code to be refined.
// TODO: Check for invalid security codes when connecting to the exchange server.
// TODO: Check for invalid security codes when retrieving ticks.
// TODO: Check if we get errors when the connection is down.
// TODO: Start a new market data stream after stopping it.

