#include <gtest/gtest.h>
#include <securities.h>

namespace {
nlohmann::json markets()
{
    return {
      {"success", true},
      {"result",
       {{{"type", "spot"},
         {"name", "BTC/USD"},
         {"quoteCurrency", "USD"},
         {"baseCurrency", "BTC"},
         {"enabled", true},
         {"minProvideSize", 0.0001},
         {"priceIncrement", 1.0},
         {"sizeIncrement", 0.0001},
         {"postOnly", false},
         {"restricted", false}},
        {{"type", "future"},
         {"name", "BTC-PERP"},
         {"underlying", "BTC"},
         {"enabled", true},
         {"minProvideSize", 0.001},
         {"priceIncrement", 1.0},
         {"sizeIncrement", 0.0001},
         {"postOnly", false},
         {"restricted", false}}}}};
}

Securities loaded()
{
    Securities securities;
    securities.LoadResponse("ftx", markets());
    return securities;
}
}

TEST(securities_test_case, IsSpotSecurity)
{
    auto securities = loaded();
    EXPECT_TRUE(securities.IsSpotSecurity("BTC/USD", "ftx"));
}

TEST(securities_test_case, IsFutureSecurity)
{
    auto securities = loaded();
    EXPECT_TRUE(securities.IsFutureSecurity("BTC-PERP", "ftx"));
}

TEST(securities_test_case, SpotSecurity)
{
    auto securities = loaded();
    const auto security = securities.SecSpot("BTC/USD", "ftx");
    EXPECT_EQ(security.product, "spot");
    EXPECT_EQ(security.source, "ftx");
    EXPECT_EQ(security.symbol, "BTC/USD");
    EXPECT_EQ(security.quoteCurrency, "USD");
    EXPECT_EQ(security.baseCurrency, "BTC");
    EXPECT_TRUE(security.enabled);
    EXPECT_DOUBLE_EQ(security.minProvideSize, 0.0001);
    EXPECT_DOUBLE_EQ(security.priceIncrement, 1.0);
    EXPECT_DOUBLE_EQ(security.sizeIncrement, 0.0001);
    EXPECT_FALSE(security.postOnly);
    EXPECT_FALSE(security.restricted);
}

TEST(securities_test_case, FutureSecurity)
{
    auto securities = loaded();
    const auto security = securities.SecFuture("BTC-PERP", "ftx");
    EXPECT_EQ(security.product, "future");
    EXPECT_EQ(security.source, "ftx");
    EXPECT_EQ(security.symbol, "BTC-PERP");
    EXPECT_EQ(security.underlying, "BTC");
    EXPECT_TRUE(security.enabled);
    EXPECT_DOUBLE_EQ(security.minProvideSize, 0.001);
    EXPECT_DOUBLE_EQ(security.priceIncrement, 1.0);
    EXPECT_DOUBLE_EQ(security.sizeIncrement, 0.0001);
    EXPECT_FALSE(security.postOnly);
    EXPECT_FALSE(security.restricted);
}

TEST(securities_test_case, RejectsDuplicateSecurityCodes)
{
    auto duplicate = markets();
    duplicate["result"].push_back(duplicate["result"][0]);
    Securities securities;
    EXPECT_THROW(securities.LoadResponse("ftx", duplicate), std::runtime_error);
}
