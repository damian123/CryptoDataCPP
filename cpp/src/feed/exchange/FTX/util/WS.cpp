#include <util/WS.h>
#include <fmt/core.h>
#include <boost/asio/ssl/host_name_verification.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
std::string host_from_uri(const std::string& uri)
{
    const auto scheme = uri.find("://");
    const auto start = scheme == std::string::npos ? 0 : scheme + 3;
    const auto end = uri.find_first_of("/:", start);
    return uri.substr(start, end == std::string::npos ? std::string::npos : end - start);
}
}

namespace util {

WS::WS()
{
    wsclient.set_access_channels(websocketpp::log::alevel::none);
    wsclient.set_error_channels(websocketpp::log::elevel::fatal);

	    wsclient.init_asio();
	    wsclient.set_open_handshake_timeout(10'000);
	    wsclient.set_close_handshake_timeout(5'000);
	    wsclient.set_pong_timeout(5'000);

	    wsclient.set_open_handler([this](websocketpp::connection_hdl hdl) {
	        if (close_requested) {
	            websocketpp::lib::error_code ec;
	            wsclient.close(hdl, websocketpp::close::status::normal,
	                           "Closing connection", ec);
	            if (ec && ec != websocketpp::error::invalid_state) {
	                std::lock_guard<std::mutex> lock(connection_mutex);
	                failure_reason = "Could not close WebSocket: " + ec.message();
	            }
	            return;
	        }
	        auto subscriptions = on_open_cb ? on_open_cb() : std::vector<json>{};
        for (auto& subscription : subscriptions) {
            wsclient.send(
              hdl, subscription.dump(), websocketpp::frame::opcode::text);
        }
    });

    wsclient.set_message_handler(
	      [this](websocketpp::connection_hdl, WSClient::message_ptr msg) {
	          json j = json::parse(msg->get_raw_payload());
	          if (on_message_cb) on_message_cb(std::move(j));
      });

    wsclient.set_close_handler([this](websocketpp::connection_hdl) {
        std::cout << "connection closed";
    });

	    wsclient.set_interrupt_handler([this](websocketpp::connection_hdl) {
	        std::lock_guard<std::mutex> lock(connection_mutex);
	        failure_reason = "WebSocket operation interrupted";
	    });

    wsclient.set_fail_handler(
	      [this](websocketpp::connection_hdl hdl) {
	          auto failed = wsclient.get_con_from_hdl(hdl);
	          std::lock_guard<std::mutex> lock(connection_mutex);
	          failure_reason = failed ? failed->get_ec().message() : "WebSocket connection failed";
	      });

	    wsclient.set_tls_init_handler([this](websocketpp::connection_hdl) {
	        auto context = websocketpp::lib::make_shared<boost::asio::ssl::context>(
	          boost::asio::ssl::context::tls_client);
	        context->set_default_verify_paths();
	        context->set_verify_mode(boost::asio::ssl::verify_peer);
	        context->set_verify_callback(
	          boost::asio::ssl::host_name_verification(host_from_uri(uri)));
	        return context;
	    });
}

void WS::configure(std::string configured_uri)
{
	    if (configured_uri.empty()) throw std::invalid_argument("WebSocket URI is required");
	    uri = std::move(configured_uri);
	    close_requested = false;
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
	    auto next_connection = wsclient.get_connection(uri, ec);
	    if (ec) {
	        throw std::runtime_error(fmt::format("Could not get connection to {}: {}", uri, ec.message()));
	    }
	    {
	        std::lock_guard<std::mutex> lock(connection_mutex);
	        connection = next_connection;
	        failure_reason.clear();
	    }
	    wsclient.connect(next_connection);
	    if (close_requested) wsclient.stop();
	    wsclient.run();
	    std::lock_guard<std::mutex> lock(connection_mutex);
	    if (!failure_reason.empty()) throw std::runtime_error(failure_reason);
}

void WS::close()
{
	    close_requested = true;
	    WSClient::connection_ptr current;
	    {
	        std::lock_guard<std::mutex> lock(connection_mutex);
	        current = connection;
	    }
	    if (!current) return;
	    websocketpp::lib::error_code ec;
	    wsclient.close(current->get_handle(), websocketpp::close::status::normal,
	                   "Closing connection", ec);
	    if (ec && ec != websocketpp::error::invalid_state) {
	        throw std::runtime_error("Could not close WebSocket: " + ec.message());
	    }
}

void WS::subscribe(std::string market, std::string channel)
{    
    json msg = { {"op", "subscribe"}, {"channel", channel}, {"market", market} }; 
	    WSClient::connection_ptr current;
	    {
	        std::lock_guard<std::mutex> lock(connection_mutex);
	        current = connection;
	    }
	    if (!current) throw std::logic_error("WebSocket is not connected");
	    auto hdl = current->get_handle();
	wsclient.send(hdl, msg.dump(), websocketpp::frame::opcode::text);
}

}
