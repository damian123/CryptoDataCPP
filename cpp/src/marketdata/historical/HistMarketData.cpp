#include <HistMarketData.h>
#include <iostream>

std::vector<ohlcv> HistMarketData::Load(std::string source,
	std::string secCode,
	int interval,
	std::optional<std::tm> startTime,
	std::optional<std::tm> endTime)
{
	std::vector<ohlcv> result;

	ftx::RESTClient client;

	if (source == "ftx")
	{
		auto data = client.get_OHLCV(secCode, interval);
		
		if (data["success"].get<bool>() != true) {
			throw std::exception("Failed to retrieve ohlcv rest message from FTX");
		}

		for (auto candle : data["result"])
		{
			// std::cout << candle.dump() << "\n\n";
			// for example
			// {"close":10594.5, "high" : 10655.0, "low" : 10330.0, "open" : 10529.0, "startTime" : "2019-07-21T00:00:00+00:00", "time" : 1563667200000.0, "volume" : 93.89585}

			ohlcv res;
			res.open = candle["open"];
			res.high = candle["high"];
			res.low = candle["low"];
			res.close = candle["close"];
			res.volume = candle["volume"];

			std::tm time = std::tm{};
			std::string dtstr = candle["startTime"];
			std::istringstream stext(dtstr.c_str());
			stext >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S");
			if (stext.fail()) {
				throw std::exception(fmt::format("Time conversion failed {} {} {}", secCode, dtstr).c_str());
			}
			else {
				res.startTime = time;				
			}

			result.push_back(res);
		}
	}
	else
	{
		throw std::exception(fmt::format("historical market data source not supported: {}", source).c_str());
	}

	return result;
}
