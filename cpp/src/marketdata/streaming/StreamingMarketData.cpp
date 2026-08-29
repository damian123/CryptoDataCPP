#include <StreamingMarketData.h>

#include <ctime>
#include <fmt/core.h>
#include <iomanip>
#include <iostream>
#include <spdlog/fmt/ostr.h>

std::ostream& operator<<(std::ostream& o, const Tick& t)
{
	using namespace std::chrono;
	const auto total = milliseconds{static_cast<std::int64_t>(t.time * 1000.)};
	const auto whole_seconds = duration_cast<seconds>(total);
	const std::time_t epoch = whole_seconds.count();
	std::tm utc{};
#ifdef _WIN32
	gmtime_s(&utc, &epoch);
#else
	gmtime_r(&epoch, &utc);
#endif
	const auto remainder = (total - duration_cast<milliseconds>(whole_seconds)).count();
	return o << std::put_time(&utc, "%Y-%m-%d %H:%M:%S") << "."
	         << std::setw(3) << std::setfill('0') << remainder << "Z,"
	         << t.bid << "," << t.ask << "," << t.last;
}

void StreamingMarketData::ApplyMessage(const json& message)
{
	const std::string type = message.at("type").get<std::string>();
	if (type == "error") {
		throw std::runtime_error(fmt::format("stream error message: {}", message.dump()));
	}
	if (type == "subscribed") {
		log_->info(fmt::format("subscribed: {}", message.dump()));
		return;
	}
	if (type != "update") {
		throw std::runtime_error(
		  fmt::format("unknown message type {}: {}", type, message.dump()));
	}
	if (message.at("channel").get<std::string>() != "ticker") {
		throw std::runtime_error("stream update is not a ticker message");
	}

	Tick tick{};
	tick.time = message.at("data").at("time").get<double>();
	tick.bid = message.at("data").at("bid").get<double>();
	tick.ask = message.at("data").at("ask").get<double>();
	tick.last = message.at("data").at("last").get<double>();
	const std::string security = message.at("market").get<std::string>();
	std::lock_guard<std::mutex> lock(marketdata_mutex_);
	marketdata_[security] = tick;
}

void StreamingMarketData::run()
{
	if (stop_) return;
	try {
		ftxClient_.subscribe_ticker(secid_);
		ftxClient_.on_message([this](json message) { ApplyMessage(message); });
		ftxClient_.connect();
	} catch (const std::exception& error) {
		log_->error(fmt::format("Connection error: {}", error.what()));
	}
	stop_ = true;
}

void StreamingMarketData::Subscribe(std::string secID)
{
	ftxClient_.subscribe(secID, "ticker");
}

Tick StreamingMarketData::getTick(std::string secID)
{
	std::lock_guard<std::mutex> lock(marketdata_mutex_);
	const auto found = marketdata_.find(secID);
	if (found == marketdata_.end()) {
		throw std::runtime_error(
		  fmt::format("Security ID {} missing in FTX market data", secID));
	}
	return found->second;
}

void StreamingMarketData::stop()
{
	stop_ = true;
	std::exception_ptr close_error;
	try {
		ftxClient_.close();
	} catch (...) {
		close_error = std::current_exception();
	}
	Runnable::stop();
	if (close_error) std::rethrow_exception(close_error);
}
