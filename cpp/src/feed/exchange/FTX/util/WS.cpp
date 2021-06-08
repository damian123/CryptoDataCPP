#include <util/WS.h>
#include <util/Secret.h>
#include <fmt/core.h>

namespace util {

WS::WS()
{
    wsclient.set_access_channels(websocketpp::log::alevel::none);
    wsclient.set_error_channels(websocketpp::log::elevel::fatal);

    wsclient.init_asio();

    wsclient.set_open_handler([this](websocketpp::connection_hdl hdl) {
        auto subscriptions = on_open_cb();
        for (auto& subscription : subscriptions) {
            wsclient.send(
              hdl, subscription.dump(), websocketpp::frame::opcode::text);
        }
    });

    wsclient.set_message_handler(
      [this](websocketpp::connection_hdl, WSClient::message_ptr msg) {
          json j = json::parse(msg->get_raw_payload());
          on_message_cb(j);
      });

    wsclient.set_close_handler([this](websocketpp::connection_hdl) {
        std::cout << "connection closed";
    });

    wsclient.set_interrupt_handler(
      [this](websocketpp::connection_hdl) { throw "Interrupt handler"; });

    wsclient.set_fail_handler(
      [this](websocketpp::connection_hdl hdl) 
        { 
            // TODO: improve the error handling  here, by writing to a log file and throw std::exception instead of a string.
            // Add unit tests to verify that this works, which might require me to setup my own mock socket server
            // auto con = wsclient.get_con_from_hdl(hdl);
			// auto server = con->get_response_header("Server");
			// auto error_reason = con->get_ec().message();            
            throw "Fail handler"; 
        });

    wsclient.set_tls_init_handler([](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(
          boost::asio::ssl::context::tlsv12);
    });
}

void WS::configure(std::string _uri,
                   std::string _api_key,
                   std::string _api_secret,
                   std::string _subaccount_name)
{
    uri = _uri;

	const std::vector<unsigned char> key = Base64Decode(_api_key.c_str());	
    api_key = std::string(key.begin(), key.end());

    const std::vector<unsigned char> secret = Base64Decode(_api_secret.c_str());
    api_secret = std::string(secret.begin(), secret.end());

    subaccount_name = _subaccount_name;
}

void WS::set_on_open_cb(OnOpenCB open_cb)
{
    on_open_cb = open_cb;
}

void WS::set_on_message_cb(OnMessageCB message_cb)
{
    on_message_cb = message_cb;
}

void WS::connect()
{
    websocketpp::lib::error_code ec;
    connection = wsclient.get_connection(uri, ec);
    if (ec) {        
        throw std::exception(fmt::format("Could not get connection to {} {}", uri, ec.message()).c_str());
    }
    wsclient.connect(connection);
    wsclient.run();
}

void WS::close()
{
    connection->close(websocketpp::close::status::normal, "Closing connection");
}

void WS::subscribe(std::string market, std::string channel)
{    
    json msg = { {"op", "subscribe"}, {"channel", channel}, {"market", market} }; 
    auto hdl = connection->get_handle();    
	wsclient.send(hdl, msg.dump(), websocketpp::frame::opcode::text);
}

}
