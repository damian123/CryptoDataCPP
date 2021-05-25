#pragma once

#include <ws/client.h>
#include <thread>
#include <atomic>
#include  <chrono>
#include <tbb/concurrent_hash_map.h>

struct Tick
{
	double time;	// Number of milliseconds since Unix epoch. UTC timezone
	double bid;
	double ask;
	double last;

	friend std::ostream& operator<<(std::ostream& o, const Tick& t);	
};

class Runnable
{
public:
	Runnable() : stop_(), thread_() { }
	virtual ~Runnable() { try { stop(); } catch (...) { /*??*/ } }

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
	void SecID(const std::string& secID) { secid_ = secID; }
	Tick getTick(const std::string& secID);
	virtual void stop() { stop_ = true; ftxClient_.stop(); }
protected:	
	void run();

private:
	std::string secid_;
    ftx::WSClient ftxClient_;
	tbb::concurrent_hash_map<std::string, Tick> marketdata_;
};
