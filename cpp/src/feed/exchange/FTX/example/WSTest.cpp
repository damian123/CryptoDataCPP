#include <ws/client.h>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;


int main()
{
	    std::cerr << "ARCHIVED EXAMPLE: this contacts the defunct FTX host; do not use credentials.\n";
	    ftx::WSClient client;
	    client.subscribe_orderbook("BTC-PERP");
    client.subscribe_ticker("BTC-PERP");

    client.on_message([](json j) { std::cout << "msg: " << j << "\n"; });

    client.connect();
}
