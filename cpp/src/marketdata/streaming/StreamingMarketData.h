#pragma once

#include <ws/client.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <tbb/concurrent_hash_map.h>
#include <spdlog/spdlog.h>

struct Tick
{
	double time;	// Number of milliseconds since Unix epoch. UTC timezone.
	double bid;
	double ask;
	double last;

	friend std::ostream& operator<<(std::ostream& o, const Tick& t);	
};

class Runnable
{
public:
	virtual ~Runnable() { try { stop(); } catch (...) { /*??*/ } }

protected:
	Runnable() : stop_(), thread_() { }

public:
	Runnable(Runnable const&) = delete;
	Runnable& operator =(Runnable const&) = delete;

	virtual void stop() { stop_ = true; thread_.join(); }
	void start() { thread_ = std::thread(&Runnable::run, this); }

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
	virtual void stop() {		
		ftxClient_.close();
		stop_ = true;
	}

protected:
	void run();

private:
	StreamingMarketData() : log_{ spdlog::get("ftx") } {};

public:
	StreamingMarketData(StreamingMarketData const&) = delete;
	StreamingMarketData& operator=(StreamingMarketData const&) = delete;

private:
	std::string secid_;
    ftx::WSClient ftxClient_;
	tbb::concurrent_hash_map<std::string, Tick> marketdata_;
	std::shared_ptr<spdlog::logger> log_;
};
