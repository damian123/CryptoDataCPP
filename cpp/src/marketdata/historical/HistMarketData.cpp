#include <HistMarketData.h>
#include <fmt/core.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace {
std::optional<std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>>
to_utc_time_point(const std::optional<std::tm>& value)
{
	if (!value) return std::nullopt;
	std::tm copy = *value;
#ifdef _WIN32
	const std::time_t converted = _mkgmtime(&copy);
#else
	const std::time_t converted = timegm(&copy);
#endif
	if (converted == static_cast<std::time_t>(-1)) {
		throw std::invalid_argument("historical filter cannot be represented as UTC");
	}
	return std::chrono::time_point_cast<std::chrono::seconds>(
	  std::chrono::system_clock::from_time_t(converted));
}
}

std::vector<ohlcv> HistMarketData::ParseResponse(
  const json& data, const std::string& security_code)
{
	if (!data.contains("success") || !data["success"].is_boolean() ||
	    !data["success"].get<bool>()) {
		throw std::runtime_error("Failed to retrieve OHLCV response");
	}
	if (!data.contains("result") || !data["result"].is_array()) {
		throw std::runtime_error("OHLCV response does not contain a result array");
	}

	std::vector<ohlcv> result;
	for (const auto& candle : data["result"]) {
		ohlcv parsed{};
		parsed.open = candle.at("open").get<double>();
		parsed.high = candle.at("high").get<double>();
		parsed.low = candle.at("low").get<double>();
		parsed.close = candle.at("close").get<double>();
		parsed.volume = candle.at("volume").get<double>();

		const std::string timestamp = candle.at("startTime").get<std::string>();
		std::istringstream text(timestamp);
		text >> std::get_time(&parsed.startTime, "%Y-%m-%dT%H:%M:%S");
		if (text.fail()) {
			throw std::runtime_error(
			  fmt::format("Time conversion failed for {}: {}", security_code, timestamp));
		}
		result.push_back(parsed);
	}
	return result;
}

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
			auto data = client.get_OHLCV(secCode, interval, 5000,
			                             to_utc_time_point(startTime),
			                             to_utc_time_point(endTime));
			result = ParseResponse(data, secCode);
	}
	else
	{
			throw std::runtime_error(
			  fmt::format("historical market data source not supported: {}", source));
	}

	return result;
}
