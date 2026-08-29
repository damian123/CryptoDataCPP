#pragma once

#include <ws/client.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <exception>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <spdlog/spdlog.h>

struct Tick
{
	double time;	// Fractional seconds since Unix epoch, UTC.
	double bid;
	double ask;
	double last;

	friend std::ostream& operator<<(std::ostream& o, const Tick& t);	
};

class Runnable
{
public:
	virtual ~Runnable() { try { stop(); } catch (...) {} }

protected:
	Runnable() : stop_(), thread_() { }

public:
	Runnable(Runnable const&) = delete;
	Runnable& operator =(Runnable const&) = delete;

	virtual void stop()
	{
		stop_ = true;
		if (thread_.joinable() && thread_.get_id() != std::this_thread::get_id()) {
			thread_.join();
		}
	}
	void start()
	{
		if (thread_.joinable()) throw std::logic_error("worker is already running");
		stop_ = false;
		thread_ = std::thread(&Runnable::run, this);
	}

protected:
	virtual void run() = 0;
	std::atomic<bool> stop_;

private:
	std::thread thread_;
};

class StreamingMarketData : public Runnable {
public:
	static StreamingMarketData& getInstance()
	{
		static StreamingMarketData instance;
		return instance;
	}

	void SecID(std::string secID) { secid_ = secID; }
	void Subscribe(std::string secID);
	Tick getTick(std::string secID);
	void ApplyMessage(const json& message);
	~StreamingMarketData() override
	{
		try {
			stop();
		} catch (...) {
			// Destructors must not terminate the process if a legacy socket close fails.
		}
	}
	void stop() override;

protected:
	void run();

private:
	StreamingMarketData()
	  : log_{spdlog::get("ftx") ? spdlog::get("ftx") : spdlog::default_logger()} {}

public:
	StreamingMarketData(StreamingMarketData const&) = delete;
	StreamingMarketData& operator=(StreamingMarketData const&) = delete;

private:
	std::string secid_;
    ftx::WSClient ftxClient_;
	std::unordered_map<std::string, Tick> marketdata_;
	mutable std::mutex marketdata_mutex_;
	std::shared_ptr<spdlog::logger> log_;
};
