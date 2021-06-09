#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <HistMarketData.h>

#define GTEST_COUT std::cerr << "[          ] "

TEST(historical_marketdata_test_case, OHLCV)
{                
    // Get the spot rate
    using namespace std::chrono_literals;

    try {
        HistMarketData hmd;
        auto ohlcv = hmd.Load("ftx", "BTC/USD");
        ASSERT_TRUE(true);        
    }
    catch (const std::exception& ex)
    {
        GTEST_COUT << ex.what()  << std::endl;
        FAIL();
    }
}
