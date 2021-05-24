#include <securities.h>
#include <iostream>
#include <fstream>
#include <fmt/core.h>

SpotSecurity Securities::SecSpot(const std::string& secCode, const std::string& source)
{
	if (source.size() > 0)
	{		
		if (spotSec_.count(source) == 0)
			throw std::exception(fmt::format("spot security source not found: {}", source).c_str());
		auto it = spotSec_[source].find(secCode);
		if (it == spotSec_[source].end()) {
			throw std::exception(fmt::format("spot security code not found: {}", secCode).c_str());
		}
		return (it->second);
	}
	else
	{
		throw std::exception(fmt::format("unknown securities source: {}", source).c_str());
	}
}

FutureSecurity Securities::SecFuture(const std::string& secCode, const std::string& source)
{
	if (source.size() > 0)
	{
		if (futureSec_.count(source) == 0)
			throw std::exception(fmt::format("future security source not found: {}", source).c_str());
		auto it = futureSec_[source].find(secCode);
		if (it == futureSec_[source].end()) {
			throw std::exception(fmt::format("future security code not found: {}", secCode).c_str());			
		}
		return (it->second);
	}
	else
	{
		throw std::exception(fmt::format("unknown securities source: {}", source).c_str());		
	}
}

bool Securities::FindSpot(const std::string& seccode, const std::string& source)
{	
	if (source.size() > 0)
		return spotSec_[source].count(seccode) > 0;
	else
	{
		// Search all the source for the securities code
		throw std::exception("not implemented");
	}
}

bool Securities::FindFuture(const std::string& seccode, const std::string& source)
{
	if (source.size() > 0)
		return futureSec_[source].count(seccode) > 0;
	else
	{
		// Search all the source for the securities code		
		throw std::exception("not implemented");
	}
}

namespace {
	void dump(json markets)
	{		
		std::ofstream f;
		f.open("securities_raw_ftx.json");
		f << markets.dump(0) << "\n\n";
		f.close();
	}

	SpotSecurity ConvertSpot(json m)
	{
		SpotSecurity s;

		s.product = "spot";
		s.source = "ftx";
		s.symbol = m["name"];		
		s.quoteCurrency = m["quoteCurrency"];
		s.baseCurrency= m["baseCurrency"];
		
		// Exchange rules
		s.enabled = m["enabled"];
		s.minProvideSize = m["minProvideSize"];
		s.priceIncrement = m["priceIncrement"];
		s.sizeIncrement = m["sizeIncrement"];
		s.postOnly = m["postOnly"];
		s.restricted = m["restricted"];

		return s;
	}

	FutureSecurity ConvertFuture(json m)
	{
		FutureSecurity f;

		f.product = "future";
		f.source = "ftx";
		f.symbol = m["name"];
		f.underlying = m["underlying"];

		// Exchange rules
		f.enabled = m["enabled"];
		f.minProvideSize = m["minProvideSize"];
		f.priceIncrement = m["priceIncrement"];
		f.sizeIncrement = m["sizeIncrement"];
		f.postOnly = m["postOnly"];
		f.restricted = m["restricted"];

		return f;
	}
}

void Securities::Refresh(const std::string& source)
{
	if (source == "ftx")
	{
		ftx::RESTClient client;
		auto markets = client.list_markets();
		//dump(markets);
		std::string type;
		std::map<std::string, SpotSecurity> spotSecdb;
		std::map<std::string, FutureSecurity> futureSecdb;

		if (markets["success"].get<bool>() != true) {
			throw std::exception("Failed to retrieve rest message from FTX");
		}
		
		for (auto m : markets["result"])
		{
			auto itType = m.find("type");
			if (itType == m.end()) {
				throw std::exception("type not found in ftx exchange data");
			}
			auto itName= m.find("name");
			if (itName == m.end()) {
				throw std::exception("name not found in ftx exchange data");
			}
			type = itType.value().get<std::string>();
			std::string secCode = itName.value().get<std::string>();
			
			if (type.compare("spot") == 0 )
			{				
				// TODO: Check for unique security codes
				spotSecdb[secCode] = ConvertSpot(m);
			}
			else if (type.compare("future") == 0)
			{
				// TODO: Check for unique security codes
				futureSecdb[secCode] = ConvertFuture(m);
			}
			else
			{
				throw std::exception(fmt::format("unknown security type: {}", type).c_str());
			}			
		}

		spotSec_["ftx"] = spotSecdb;
		futureSec_["ftx"] = futureSecdb;		
	}
	else
	{
		throw std::exception("securities source not supported");
	}
}

