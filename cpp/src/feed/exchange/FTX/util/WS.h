#pragma once

#include <json.hpp>
#include <functional>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

using json = nlohmann::json;

namespace util {

class WS
{
  public:
    using WSClient = websocketpp::client<websocketpp::config::asio_tls_client>;
    using OnOpenCB = std::function<std::vector<json>()>;
    using OnMessageCB = std::function<void(json j)>;

    //! Default constructor.
    WS();

    /*!
     * Configuration of the websocket.     
	 * Each sub account has independent balances, margin, and positions, and can trade independently.
	 * Sub accounts are useful for isolating positions, margin, or access.
     * @param _url  The URL of the exchange server.
     * @param _api_key  The account's API key.
     * @param _api_secret The account's API secret.
     * @param _subaccount_name The sub account name.
     */
    void configure(std::string _uri,
                   std::string _api_key,
                   std::string _api_secret,
                   std::string _subaccount_name);

    /*!
     * Set a callback function that is called when a web socket connection is opened.
	 * The callback function returns a set of messages that allows for exchange server authentication,
     * i.e. the api key, secret and sub account, and the channels to subscribe to for each market.
     * @param open_cb The callback function.
     */
    void set_on_open_cb(OnOpenCB open_cb);

    /*!
     * Set a callback function to call when a message arrives on the subscription channel.
     * @param message_cb
     */
    void set_on_message_cb(OnMessageCB message_cb);

    /*!
     * Connect to the socket endpoint and start the event processing loop.
     * Establish a socket connection to an endpoint and run the io_context event processing loop 
     * in the I/O service object that links to the OS's I/O services. 
     * As soon this function is called we would typically be called back asynchronous into 
     * the on message callback function we have specified earlier.
     */
    void connect();

    /*!
    * Close the connection. 
    * This will cleanly perform the WebSocket and TCP closing handshakes (with a configurable timeout 
    * to make sure a malicious or broken client doesn't hang forever). The cleanup process is not instantaneous, 
    * as it requires a few network round trips to clean everything up, as a result you can get a 
    * few more async message callbacks.
    */
    void close();

    void subscribe(std::string market, std::string channel);

  private:
    WSClient wsclient;
    WSClient::connection_ptr connection;
    OnOpenCB on_open_cb;
    OnMessageCB on_message_cb;
    std::string uri;
    std::string api_key;
    std::string api_secret;
    std::string subaccount_name;
};

}
