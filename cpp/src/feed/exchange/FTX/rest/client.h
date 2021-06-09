#pragma once

#include "util/HTTP.h"
#include <json.hpp>
#include <string>
#include <chrono>

using json = nlohmann::json;

namespace ftx {

class RESTClient
{
  public:
    //! Default constructor
    RESTClient();

    /*! 
     * List all futures on FTX.
     * @return Market data and instrument spec for all listed futures.
     */    
    json list_futures();
    
    /*! List all markets on FTX, where a market is a listed security. 
     * @return A set of markets showing market data and instrument spec.
     */ 
    json list_markets();
    
    /*!
     * Get the order book for a market.
     * @param market
     * @param depth
     * @return The list of orders in the order book.
     */
    json get_orderbook(const std::string market, int depth = 100);

    /*!
     * Get trades for a market (security code)
     * @param market
     * @return The users executed trades for the market.
     */
    json get_trades(const std::string market);

    /*!
     * Get historical OHLCV data.
     * For all securities on FTX including futures, perpetuals, spot, and MOVE contracts.
     * For historical prices of expired futures please make sure to specify start time and end time.
     * @param market
     * @param window is the length of the time window in seconds. Default is 300 seconds, but other options are 15, 60, 300, 900, 3600, 14400, 86400.
     * @param limit is the maximum 
     * @param startTime is the requested start time in UTC time zone format.
     * @param endTime is the requested end time in UTC time zone format.
     * 
     * @return The OHLCV response 
     */
    json get_OHLCV(const std::string market,
        int window = 300,
        int limit = 5000,
        std::optional<std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>> startTime = {},
        std::optional<std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>> endTime = {});

    /*!
     * Get account information.
     * @return The account information for the user.
     */
    json get_account_info();

    /*!
     * Get open orders. 
     * @return
     */
    json get_open_orders();

    /*!
     * Place an limit order on the exchange.
     * @param market The symbol code for the security.
     * @param side buy or sell.
     * @param price The price limit.
     * @param size The notional amount of base currency.
     * @param ioc true if "immediate or cancel" order type, else false.
     * @param post_only true if the order will not be allowed to take. Post-only orders will only pay maker fees. The default value is false.
     * @param reduce_only true if the order will only reduce your position size. These orders can only close your positions. The default value is false.
     * @return
     */    
    json place_order(const std::string market,
                     const std::string side,
                     double price,
                     double size,
                     bool ioc = false,
                     bool post_only = false,
                     bool reduce_only = false);
    
    /*!
     * Place a market order on the exchange.
     * @param market The symbol code for the security.
     * @param side buy or sell
     * @param size The notional amount of base currency
     * @param ioc true if "immediate or cancel" order type, else false
	 * @param post_only true if the order will not be allowed to take. Post-only orders will only pay maker fees. The default value is false.
	 * @param reduce_only true if the order will only reduce your position size. These orders can only close your positions. The default value is false.
     * @return
     */        
    json place_order(const std::string market,
                     const std::string side,
                     double size,
                     bool ioc = false,
                     bool post_only = false,
                     bool reduce_only = false);

    /*!
     * Cancel an order.
     * @param order_id
     * @return The order queued for cancellation.
     */
    json cancel_order(const std::string order_id);

    /*!
     * Get filled orders
     * @return Fills for the executed orders.
     */
    json get_fills();

    /*!
     * Get wallet balances
     * @return The asset balances for the wallet
     */
    json get_balances();

    /*!
     * Get wallet deposit address for a ticker code.
     * @param ticker The symbol code for security.
     * @return
     */
    json get_deposit_address(const std::string ticker);

  private:
    util::HTTPSession http_client;
    const std::string uri = "ftx.com";
    const std::string api_key = "";
    const std::string api_secret = "";
    const std::string subaccount_name = "";
};

}
