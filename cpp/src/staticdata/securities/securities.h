#pragma once

#include <string>
#include <map>

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
	SpotSecurity SecSpot(std::string secCode, std::string source = "");
	FutureSecurity SecFuture(std::string secCode, std::string source = "");
	bool IsSpotSecurity(std::string seccode, std::string source = "");	
	bool IsFutureSecurity(std::string seccode, std::string source = "");
	void Refresh(std::string source);
private:
	// map of source to map of security code to description
	std::map <std::string, std::map<std::string, SpotSecurity>> spotSec_;
	std::map<std::string, std::map<std::string, FutureSecurity>> futureSec_;
};
