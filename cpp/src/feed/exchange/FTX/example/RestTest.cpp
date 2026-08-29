#include <rest/client.h>

#include <iostream>

int main()
{
	    std::cerr << "ARCHIVED EXAMPLE: this contacts the defunct FTX host; do not use credentials.\n";
	    ftx::RESTClient client;

    auto futs = client.list_futures();
    std::cout << futs << "\n";

	    auto markets = client.list_markets();
    std::cout << markets << "\n";

    auto ob = client.get_orderbook("BTC-PERP", 3);
    std::cout << ob << "\n\n";

	auto ohlcv = client.get_OHLCV("BTC-PERP", 86400);
	std::cout << ohlcv << "\n";

}
