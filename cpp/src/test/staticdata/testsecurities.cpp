#include <iostream>
#include <gtest/gtest.h>
#include <securities.h>

TEST(securities_test_case, FindSpot)
{            
    Securities s;
    s.Refresh("ftx");
    EXPECT_EQ(true, s.FindSpot("BTC/USD", "ftx"));
}

TEST(securities_test_case, FindFuture)
{
	Securities s;
	s.Refresh("ftx");
	EXPECT_EQ(true, s.FindFuture("BTC-PERP", "ftx"));
}

TEST(securities_test_case, SpotSecurity)
{
	Securities s;
	s.Refresh("ftx");
	auto sec = s.SecSpot("BTC/USD", "ftx");
	EXPECT_STREQ("spot", sec.product.c_str());
	EXPECT_STREQ("ftx", sec.source.c_str());
	EXPECT_STREQ("BTC/USD", sec.symbol.c_str());
	EXPECT_STREQ("USD", sec.quoteCurrency.c_str());
	EXPECT_STREQ("BTC", sec.baseCurrency.c_str());	
	// Validate Exchange Rules
	EXPECT_EQ(true, sec.enabled);						
	EXPECT_EQ(0.0001, sec.minProvideSize);
	EXPECT_EQ(1.0, sec.priceIncrement);
	EXPECT_EQ(0.0001, sec.sizeIncrement);
	EXPECT_EQ(false, sec.postOnly);
	EXPECT_EQ(false, sec.restricted);
}

TEST(securities_test_case, FutureSecurity)
{
	Securities s;
	s.Refresh("ftx");
	auto sec = s.SecFuture("BTC-PERP", "ftx");
	EXPECT_STREQ("future", sec.product.c_str());
	EXPECT_STREQ("ftx", sec.source.c_str());
	EXPECT_STREQ("BTC-PERP", sec.symbol.c_str());		
	// Validate Exchange Rules
	EXPECT_EQ(true, sec.enabled);
	EXPECT_EQ(0.001, sec.minProvideSize);
	EXPECT_EQ(1.0, sec.priceIncrement);
	EXPECT_EQ(0.0001, sec.sizeIncrement);
	EXPECT_EQ(false, sec.postOnly);
	EXPECT_EQ(false, sec.restricted);
}
