#pragma once

#include <string>
#include <map>
#include <rest/client.h>

struct ExchangeRules
{
	bool enabled;
	double minProvideSize;
	double priceIncrement;
	double sizeIncrement;
	bool postOnly;
	bool restricted;
};

struct SpotSecurity : public ExchangeRules
{
	std::string product;
	std::string source;
	std::string symbol;
	std::string quoteCurrency;
	std::string baseCurrency;	
};

struct FutureSecurity  : public ExchangeRules
 {
	std::string product;
	std::string source;
	std::string symbol;
	std::string underlying;
 };

class Securities
{
public:
	SpotSecurity SecSpot(const std::string& secCode, const std::string& source = "");
	FutureSecurity SecFuture(const std::string& secCode, const std::string& source = "");
	bool FindSpot(const std::string& seccode, const std::string& source = "");	
	bool FindFuture(const std::string& seccode, const std::string& source = "");
	void Refresh(const std::string& source);
private:
	// map of source to map of security code to description
	std::map <std::string, std::map<std::string, SpotSecurity>> spotSec_;
	std::map<std::string, std::map<std::string, FutureSecurity>> futureSec_;
};

