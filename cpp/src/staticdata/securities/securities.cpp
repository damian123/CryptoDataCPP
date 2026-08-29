#include <securities.h>
#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include <rest/client.h>
#include <stdexcept>

SpotSecurity Securities::SecSpot(std::string secCode, std::string source)
{
	if (source.size() > 0)
	{		
		if (spotSec_.count(source) == 0)
				throw std::runtime_error(fmt::format("spot security source not found: {}", source));
		auto it = spotSec_[source].find(secCode);
		if (it == spotSec_[source].end()) {
				throw std::runtime_error(fmt::format("spot security code not found: {}", secCode));
		}
		return (it->second);
	}
	else
	{
		throw std::runtime_error(fmt::format("unknown securities source: {}", source));
	}
}

FutureSecurity Securities::SecFuture(std::string secCode, std::string source)
{
	if (source.size() > 0)
	{
		if (futureSec_.count(source) == 0)
				throw std::runtime_error(fmt::format("future security source not found: {}", source));
		auto it = futureSec_[source].find(secCode);
		if (it == futureSec_[source].end()) {
				throw std::runtime_error(fmt::format("future security code not found: {}", secCode));
		}
		return (it->second);
	}
	else
	{
		throw std::runtime_error(fmt::format("unknown securities source: {}", source));
	}
}

bool Securities::IsSpotSecurity(std::string seccode, std::string source)
{	
	if (source.size() > 0)
		{
			auto source_it = spotSec_.find(source);
			return source_it != spotSec_.end() && source_it->second.count(seccode) > 0;
		}
	else
	{
		// Search all the source for the securities code
			throw std::logic_error("searching every securities source is not implemented");
	}
}

bool Securities::IsFutureSecurity(std::string seccode, std::string source)
{
	if (source.size() > 0)
		{
			auto source_it = futureSec_.find(source);
			return source_it != futureSec_.end() && source_it->second.count(seccode) > 0;
		}
	else
	{
		// Search all the source for the securities code		
			throw std::logic_error("searching every securities source is not implemented");
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

void Securities::Refresh(std::string source)
{
	if (source == "ftx")
	{
		ftx::RESTClient client;
		auto markets = client.list_markets();
		LoadResponse(source, markets);
	}
	else
	{
		throw std::runtime_error(fmt::format("securities source not supported: {}", source));
	}
}

void Securities::LoadResponse(const std::string& source, const nlohmann::json& markets)
{
	if (source.empty()) throw std::invalid_argument("securities source is required");
	if (!markets.contains("success") || !markets["success"].is_boolean() ||
	    !markets["success"].get<bool>()) {
		throw std::runtime_error("market response was not successful");
	}
	if (!markets.contains("result") || !markets["result"].is_array()) {
		throw std::runtime_error("market response does not contain a result array");
	}

	std::map<std::string, SpotSecurity> spot;
	std::map<std::string, FutureSecurity> futures;
	for (const auto& market : markets["result"]) {
		const std::string type = market.at("type").get<std::string>();
		const std::string code = market.at("name").get<std::string>();
		if (type == "spot") {
			if (!spot.emplace(code, ConvertSpot(market)).second) {
				throw std::runtime_error(fmt::format("duplicate spot security code: {}", code));
			}
		} else if (type == "future") {
			if (!futures.emplace(code, ConvertFuture(market)).second) {
				throw std::runtime_error(fmt::format("duplicate future security code: {}", code));
			}
		} else {
			throw std::runtime_error(fmt::format("unknown security type: {}", type));
		}
	}
	spotSec_[source] = std::move(spot);
	futureSec_[source] = std::move(futures);
}
