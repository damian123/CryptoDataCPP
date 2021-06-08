#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <StreamingMarketData.h>

#define GTEST_COUT std::cerr << "[          ] "

TEST(streaming_marketdata_test_case, SpotRate)
{                
    // Get the spot rate
    using namespace std::chrono_literals;

    try {
        // BTC/USD spot rates are already subscribed to in the test SetUp()
        Tick t = StreamingMarketData::getInstance().getTick("BTC/USD");
        std::cout << t << "\n"; 
        ASSERT_TRUE(true);
        
    }
    catch (const std::exception& ex)
    {
        GTEST_COUT << ex.what()  << std::endl;
        FAIL();
    }
}

TEST(streaming_marketdata_test_case, Subscribe)
{
    // Subscribe to spot market data
	using namespace std::chrono_literals;

	try {
        StreamingMarketData::getInstance().Subscribe("ETH/USD");
		std::this_thread::sleep_for(0.5s); // Instead of this: Async wait for confirmation from the exchange and the first market data to arrive.
		Tick t = StreamingMarketData::getInstance().getTick("ETH/USD");
		std::cout << t << "\n";
		ASSERT_TRUE(true);

	}
	catch (const std::exception& ex)
	{
		GTEST_COUT << ex.what() << std::endl;
		FAIL();
	}
}


// Some of these the market feed code to be refined.
// TODO: Check for invalid security codes when connecting to the exchange server.
// TODO: Check for invalid security codes when retrieving ticks.
// TODO: Check if we get errors when the connection is down.
// TODO: Start a new market data stream after stopping it.

