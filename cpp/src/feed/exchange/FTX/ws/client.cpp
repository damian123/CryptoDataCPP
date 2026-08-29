#include "ws/client.h"
#include "util/Encoding.h"
#include "util/TimeUtil.h"
#include <stdexcept>
#include <utility>

namespace encoding = util::encoding;

namespace ftx {

WSClient::WSClient(std::string uri,
                   std::string api_key,
                   std::string api_secret,
                   std::string subaccount_name)
  : uri(std::move(uri)),
    api_key(std::move(api_key)),
    api_secret(std::move(api_secret)),
    subaccount_name(std::move(subaccount_name))
{
	    if (this->api_key.empty() != this->api_secret.empty()) {
	        throw std::invalid_argument("API key and secret must be provided together");
	    }
	    ws.configure(this->uri);
    ws.set_on_open_cb([this]() { return this->on_open(); });
}

void WSClient::on_message(util::WS::OnMessageCB cb)
{
    ws.set_on_message_cb(cb);
}

void WSClient::connect()
{
    ws.connect();
}

void WSClient::close()
{
	ws.close();
}

std::vector<json> WSClient::on_open()
{
    std::vector<json> msgs;

    if (!(api_key.empty() || api_secret.empty())) {
	        const auto ts = util::get_ms_timestamp(util::current_time()).count();
        std::string data = std::to_string(ts) + "websocket_login";
	        std::string hmacced = encoding::hmac_sha256(api_secret, data);
	        std::string sign =
	          encoding::util_string_to_hex(
	            reinterpret_cast<const unsigned char*>(hmacced.data()), hmacced.size());
        json msg = {{"op", "login"},
                    {"args", {{"key", api_key}, {"sign", sign}, {"time", ts}}}};
	        if (!subaccount_name.empty()) {
	            msg["args"]["subaccount"] = subaccount_name;
	        }
        msgs.push_back(msg);
    }

    for (auto& [market, channel] : subscriptions) {
        json msg = {
          {"op", "subscribe"}, {"channel", channel}, {"market", market}};
        msgs.push_back(msg);
    }

    return msgs;
}

void WSClient::subscribe_orders(std::string market)
{
    subscriptions.push_back(std::make_pair(market, "orders"));
}

void WSClient::subscribe_orderbook(std::string market)
{
    subscriptions.push_back(std::make_pair(market, "orderbook"));
}

void WSClient::subscribe_fills(std::string market)
{
    subscriptions.push_back(std::make_pair(market, "fills"));
}

void WSClient::subscribe_trades(std::string market)
{
    subscriptions.push_back(std::make_pair(market, "trades"));
}

void WSClient::subscribe_ticker(std::string market)
{
    subscriptions.push_back(std::make_pair(market, "ticker"));
}

void WSClient::subscribe(std::string market, std::string channel)
{
    ws.subscribe(market, channel);
}

}
