#pragma once

#include "util/HTTP.h"
#include <json.hpp>
#include <string>

using json = nlohmann::json;

namespace ftx {

class RESTClient
{
  public:
    RESTClient();

    /// <summary>
    /// List all futures on FTX
    /// </summary>
    /// <returns></returns>
    json list_futures();

    /// <summary>
    /// List all markets on FTX
    /// </summary>
    /// <returns></returns>
    json list_markets();

    /// <summary>
    /// Get the order book for a market
    /// </summary>
    /// <param name="market"></param>
    /// <param name="depth"></param>
    /// <returns></returns>
    json get_orderbook(const std::string market, int depth = 100);

    /// <summary>
    /// Get trades for a market (security code)
    /// </summary>
    /// <param name="market"></param>
    /// <returns></returns>
    json get_trades(const std::string market);

    /// <summary>
    /// Get account information. 
    /// </summary>
    /// <returns></returns>
    json get_account_info();

    /// <summary>
    /// Get open orders. 
    /// </summary>
    /// <returns></returns>
    json get_open_orders();

    /// <summary>
    /// Place an limit order on the exchange
    /// </summary>
    /// <param name="market">symbol code for the security</param>
    /// <param name="side">buy or sell</param>
    /// <param name="price">limit price</param>
    /// <param name="size">notional amount of base currency to trade</param>
    /// <param name="ioc">true if "immediate or cancel" order type, else false</param>
    /// <param name="post_only">true if the order will not be allowed to take. Post-only orders will only pay maker fees. The default value is false.</param>
    /// <param name="reduce_only">true if the order will only reduce your position size. These orders can only close your positions. The default value is false.</param>
    /// <returns></returns>
    json place_order(const std::string market,
                     const std::string side,
                     double price,
                     double size,
                     bool ioc = false,
                     bool post_only = false,
                     bool reduce_only = false);
    
    /// <summary>
    /// Place a market order on the exchange
    /// </summary>
    /// <param name="market">symbol code for the security</param>
    /// <param name="side">buy or sell</param>
    /// <param name="size">notional amount of base currency to trade</param>
    /// <param name="ioc">true if "immediate or cancel" order type, else false</param>
    /// <param name="post_only">true if the order will not be allowed to take. Post-only orders will only pay maker fees. The default value is false.</param>
    /// <param name="reduce_only">true if the order will only reduce your position size. These orders can only close your positions. The default value is false.</param>
    /// <returns></returns>
    json place_order(const std::string market,
                     const std::string side,
                     double size,
                     bool ioc = false,
                     bool post_only = false,
                     bool reduce_only = false);

    /// <summary>
    /// Cancle an order
    /// </summary>
    /// <param name="order_id"></param>
    /// <returns></returns>
    json cancel_order(const std::string order_id);

    /// <summary>
    /// Get filled orders
    /// </summary>
    /// <returns></returns>
    json get_fills();

    /// <summary>
    /// Get wallet balances
    /// </summary>
    /// <returns></returns>
    json get_balances();

    /// <summary>
    /// Get wallet deposit address
    /// </summary>
    /// <param name="ticker">symbol code for security</param>
    /// <returns></returns>
    json get_deposit_address(const std::string ticker);

  private:
    util::HTTPSession http_client;
    const std::string uri = "ftx.com";
    const std::string api_key = "";
    const std::string api_secret = "";
    const std::string subaccount_name = "";
};

}
