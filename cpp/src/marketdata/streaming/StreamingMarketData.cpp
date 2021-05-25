#include <StreamingMarketData.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <date/date.h>
#include <chrono>
#include <iostream>
#include <fmt/core.h>

typedef tbb::concurrent_hash_map<std::string, Tick>::accessor dictAccessor;

std::ostream& operator<<(std::ostream& o, const Tick& t)
{
	using namespace date;	
	using namespace std::chrono;
	sys_time<milliseconds> tp{ milliseconds{int64_t(t.time*1000.)} };
	return o << tp << "," << t.bid << "," << t.ask << "," << t.last;
}

void StreamingMarketData::run()
{ 	
	while (!stop_) 
	{ 
		ftxClient_.subscribe_ticker(secid_.c_str());

		ftxClient_.on_message(
			[this](json j)
			{
				std::cout << "msg: " << j << "\n";

				std::string type = j["type"].get<std::string>();
				if (type.compare("error") == 0)
				{
					throw std::exception(fmt::format("error message: {}", j.dump()).c_str());
				}
				else if (type.compare("subscribed") == 0)
				{
					// write this in the log file
				}
				else if (type.compare("update") == 0)
				{
					// For example this will arrive
					// {"channel":"ticker", 
					//  "data" : {"ask":36625.0, "askSize" : 0.0268, "bid" : 36616.0, "bidSize" : 0.1874, "last" : 36613.0, "time" : 1621840074.4480262}, 
					//  "market" : "BTC/USD", 
					//  "type" : "update"}
					std::string secID = j["market"];
					std::string channel = j["channel"];
					// TODO: Check channel is a ticker
					
					Tick t;
					t.time = j["data"]["time"];
					t.bid  = j["data"]["bid"];
					t.ask  = j["data"]["ask"];
					t.last = j["data"]["last"];
					dictAccessor accessor;
					marketdata_.insert(accessor, secID);
					accessor->second = t;
				}				
				else
				{
					throw std::exception(fmt::format("Unknown message type {} message: {}", type, j.dump()).c_str());
				}
			});

		ftxClient_.connect();
	};
}

Tick StreamingMarketData::getTick(const std::string& secID)
{
	dictAccessor accessor;
	const auto isFound = marketdata_.find(accessor, secID);
	if (isFound != true) {
		throw std::exception(fmt::format("Security ID {} missing in market data", secID).c_str());
	}

	return accessor->second;
}


