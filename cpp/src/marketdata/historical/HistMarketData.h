#pragma once

#include <rest/client.h>
#include <chrono>

struct ohlcv
{
	std::tm startTime;
	double open;
	double close;
	double high;
	double low;
	double volume;
};

struct HistMarketData
{
	static std::vector<ohlcv> ParseResponse(const json& response,
	                                       const std::string& security_code);
	std::vector<ohlcv> Load(std::string source, 
		std::string secCode,
		int interval = 86400,
		std::optional<std::tm> startTime = {},
		std::optional<std::tm> endTime = {});
private:
	// map of source to map of security code to Open, High, Low, Close, Volume (OHLCV) candles
	// std::map <std::string, std::map<std::string, ohlcv>> ohlcv_;
};
