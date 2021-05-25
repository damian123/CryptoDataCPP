#pragma once

#include "util/WS.h"
#include <json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace ftx {

class WSClient
{
  public:
    //! Default constructor
    WSClient();

    /*!
     * Set a function to called back when a message is received from the exchange server.
     * @parm cb the callback function.
     */
    void on_message(util::WS::OnMessageCB cb);

    /*!
    * Connect to the socket endpoint, start the event processing loop that will us callback as messages arrive.
    */
    void connect();

    /*!
    * Stop the event processing loop that will us callback as messages arrive.
    */
    void stop();

    /*!
	 * Construct messages that will be sent to the exchange server when we open the rest connection.
     * The messages specify the authentication and channels to subscribe to for each market.	 
     * @result a vector of messages 
     */
    std::vector<json> on_open();

    /*! 
     * Subscribe to the users orders for a market.
     * @param market The security code to subscribe to.
     */
    void subscribe_orders(std::string market);

    /*!
     * Subscribe to the exchanges order book.
     * Initially we get a snapshot and after that updates messages will arrive
     * @param market The security code to subscribe to.
     */
    void subscribe_orderbook(std::string market);

    /*!
     * Subscribe to a users fills for his executed orders.
     * @param market The security code to subscribe to.
     */
    void subscribe_fills(std::string market);

    /*!
     * Subscribe to a all trades on the exchanges for a market.
     * @param market The security code to subscribe to.
     */
    void subscribe_trades(std::string market);

    /*!
     * Subscribe to tick data for a market, i.e. the best bid and offer price.
     * @param market The security code to subscribe to.
     */
    void subscribe_ticker(std::string market);

  private:
    std::vector<std::pair<std::string, std::string>> subscriptions; // vector of pairs for markets and channels.
    util::WS::OnMessageCB message_cb;
    util::WS ws;
    const std::string uri = "wss://ftx.com/ws/";
    const std::string api_key = "";
    const std::string api_secret = "";
    const std::string subaccount_name = "";
};

}
