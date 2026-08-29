#include <gtest/gtest.h>
#include <StreamingMarketData.h>
#include <rest/client.h>
#include <util/Encoding.h>

#include <cstdint>
#include <limits>

TEST(streaming_marketdata_test_case, AppliesTickerFixture)
{
    auto& feed = StreamingMarketData::getInstance();
    feed.ApplyMessage({{"type", "update"},
                       {"channel", "ticker"},
                       {"market", "BTC/USD"},
                       {"data", {{"time", 1621840074.25},
                                 {"bid", 100.0},
                                 {"ask", 101.0},
                                 {"last", 100.5}}}});
    const Tick tick = feed.getTick("BTC/USD");
    EXPECT_DOUBLE_EQ(tick.bid, 100.0);
    EXPECT_DOUBLE_EQ(tick.ask, 101.0);
    EXPECT_DOUBLE_EQ(tick.last, 100.5);
}

TEST(streaming_marketdata_test_case, BuildsSubscriptionOffline)
{
    ftx::WSClient client;
    client.subscribe_ticker("ETH/USD");
    const auto messages = client.on_open();
    ASSERT_EQ(messages.size(), 1U);
    EXPECT_EQ(messages[0].at("op"), "subscribe");
    EXPECT_EQ(messages[0].at("channel"), "ticker");
    EXPECT_EQ(messages[0].at("market"), "ETH/USD");
}

TEST(rest_client_test_case, MarketOrderUsesJsonNullPrice)
{
    const auto payload = ftx::RESTClient::market_order_payload(
      "BTC-PERP", "buy", 0.01, false, false, false);
    EXPECT_TRUE(payload.at("price").is_null());
}

TEST(encoding_test_case, HmacSha256KnownVector)
{
    const auto digest = util::encoding::hmac_sha256(
      "key", "The quick brown fox jumps over the lazy dog");
    const auto hex = util::encoding::util_string_to_hex(
      reinterpret_cast<const unsigned char*>(digest.data()), digest.size());
    EXPECT_EQ(hex, "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8");
}

TEST(ws_client_test_case, AuthenticationUsesEpochMillisecondsWiderThanLongOnWindows)
{
    ftx::WSClient client(
      "wss://example.invalid/ws", "api-key", "api-secret", "archive-subaccount");
    const auto messages = client.on_open();

    ASSERT_EQ(messages.size(), 1U);
    const auto& args = messages[0].at("args");
    const auto timestamp = args.at("time").get<std::int64_t>();
    EXPECT_GT(timestamp, static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()));
    EXPECT_EQ(args.at("sign").get<std::string>().size(), 64U);
    EXPECT_EQ(args.at("subaccount"), "archive-subaccount");
    EXPECT_FALSE(messages[0].contains("subaccount"));
}
