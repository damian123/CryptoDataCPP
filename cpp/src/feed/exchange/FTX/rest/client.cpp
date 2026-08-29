#include "rest/client.h"

namespace ftx {

RESTClient::RESTClient(std::string uri,
                       std::string api_key,
                       std::string api_secret,
                       std::string subaccount_name)
  : uri(std::move(uri)),
    api_key(std::move(api_key)),
    api_secret(std::move(api_secret)),
    subaccount_name(std::move(subaccount_name))
{
	    http_client.configure(uri, api_key, api_secret, subaccount_name);
}

json RESTClient::limit_order_payload(const std::string& market,
                                     const std::string& side,
                                     double price,
                                     double size,
                                     bool ioc,
                                     bool post_only,
                                     bool reduce_only)
{
    return {{"market", market},
            {"side", side},
            {"price", price},
            {"type", "limit"},
            {"size", size},
            {"ioc", ioc},
            {"postOnly", post_only},
            {"reduceOnly", reduce_only}};
}

json RESTClient::market_order_payload(const std::string& market,
                                      const std::string& side,
                                      double size,
                                      bool ioc,
                                      bool post_only,
                                      bool reduce_only)
{
    return {{"market", market},
            {"side", side},
            {"price", nullptr},
            {"type", "market"},
            {"size", size},
            {"ioc", ioc},
            {"postOnly", post_only},
            {"reduceOnly", reduce_only}};
}

std::string RESTClient::ohlcv_target(
  const std::string& market,
  int window,
  int limit,
  std::optional<std::chrono::time_point<std::chrono::system_clock,
                                        std::chrono::seconds>> start_time,
  std::optional<std::chrono::time_point<std::chrono::system_clock,
                                        std::chrono::seconds>> end_time)
{
    std::string target =
      "markets/" + market + "/candles?resolution=" + std::to_string(window);
    target += "&limit=" + std::to_string(limit);
    if (start_time) {
        target += "&start_time=" +
                  std::to_string(start_time->time_since_epoch().count());
    }
    if (end_time) {
        target +=
          "&end_time=" + std::to_string(end_time->time_since_epoch().count());
    }
    return target;
}

json RESTClient::list_futures()
{
    auto response = http_client.get("futures");
    return json::parse(response.body());
}

json RESTClient::list_markets()
{
    auto response = http_client.get("markets");
    return json::parse(response.body());
}

json RESTClient::get_orderbook(const std::string market, int depth)
{	
    auto response =
      http_client.get("markets/" + market + "?depth=" + std::to_string(depth));
    return json::parse(response.body());
}

json RESTClient::get_trades(const std::string market)
{
    auto response = http_client.get("markets/" + market + "/trades");
    return json::parse(response.body());
}

json RESTClient::get_OHLCV(const std::string market, 
    int window,
    int limit, 
	std::optional<std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>> startTime,
	std::optional<std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>> endTime)
{
    // GET /markets/{market_name}/candles?resolution={resolution}&start_time={start_time}&end_time={end_time}
    // for example https://ftx.com/api/markets/BTC-PERP/candles?resolution=900&limit=5000&start_time=1613742300&end_time=1613745900

		auto response = http_client.get(
		  ohlcv_target(market, window, limit, startTime, endTime));
		return json::parse(response.body());
}

json RESTClient::get_account_info()
{
    auto response = http_client.get("account");
    return json::parse(response.body());
}

json RESTClient::get_open_orders()
{
    auto response = http_client.get("orders");
    return json::parse(response.body());
}

json RESTClient::place_order(const std::string market,
                             const std::string side,
                             double price,
                             double size,
                             bool ioc,
                             bool post_only,
                             bool reduce_only)
{
	    json payload = limit_order_payload(
	      market, side, price, size, ioc, post_only, reduce_only);
    auto response = http_client.post("orders", payload.dump());
    return json::parse(response.body());
}

json RESTClient::place_order(const std::string market,
                             const std::string side,
                             double size,
                             bool ioc,
                             bool post_only,
                             bool reduce_only)
{
	    json payload = market_order_payload(
	      market, side, size, ioc, post_only, reduce_only);
    auto response = http_client.post("orders", payload.dump());
    return json::parse(response.body());
}

json RESTClient::cancel_order(const std::string order_id)
{
    auto response = http_client.delete_("orders/" + order_id);
    return json::parse(response.body());
}

json RESTClient::get_fills()
{
    auto response = http_client.get("fills");
    return json::parse(response.body());
}

json RESTClient::get_balances()
{
    auto response = http_client.get("wallet/balances");
    return json::parse(response.body());
}

json RESTClient::get_deposit_address(const std::string ticker)
{
    auto response = http_client.get("wallet/deposit_address/" + ticker);
    return json::parse(response.body());
}

}
