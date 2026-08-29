#include <gtest/gtest.h>
#include <HistMarketData.h>
#include <rest/client.h>

#include <chrono>

TEST(historical_marketdata_test_case, ParsesOhlcvFixture)
{
    const json response = {
      {"success", true},
      {"result", {{{"open", 100.0},
                    {"high", 110.0},
                    {"low", 90.0},
                    {"close", 105.0},
                    {"volume", 12.5},
                    {"startTime", "2021-05-24T00:00:00+00:00"}}}}};
    const auto candles = HistMarketData::ParseResponse(response, "BTC/USD");
    ASSERT_EQ(candles.size(), 1U);
    EXPECT_DOUBLE_EQ(candles[0].open, 100.0);
    EXPECT_DOUBLE_EQ(candles[0].close, 105.0);
    EXPECT_EQ(candles[0].startTime.tm_year, 121);
}

TEST(historical_marketdata_test_case, RejectsMalformedTimestamp)
{
    const json response = {
      {"success", true},
      {"result", {{{"open", 1.0},
                    {"high", 1.0},
                    {"low", 1.0},
                    {"close", 1.0},
                    {"volume", 1.0},
                    {"startTime", "not-a-time"}}}}};
    EXPECT_THROW(HistMarketData::ParseResponse(response, "BTC/USD"), std::runtime_error);
}

TEST(historical_marketdata_test_case, IncludesRequestedDateFiltersAsEpochSeconds)
{
    using SecondsTimePoint =
      std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
    const SecondsTimePoint start{std::chrono::seconds{1'613'742'300}};
    const SecondsTimePoint end{std::chrono::seconds{1'613'745'900}};

    EXPECT_EQ(ftx::RESTClient::ohlcv_target("BTC-PERP", 900, 5000, start, end),
              "markets/BTC-PERP/candles?resolution=900&limit=5000"
              "&start_time=1613742300&end_time=1613745900");
}
