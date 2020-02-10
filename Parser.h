#pragma once
#include <cstdint>
#include <map>
#include "IEEE80211.h"

class Parser
{
private:
	struct ParseResult {
		MacAddress bssid;
		int8_t pwr;
		size_t beacons;
		size_t data;
		size_t per_second;
		uint32_t channel;
		int mb;
		int enc;
		uint8_t cipher;
		uint8_t auth;
		std::string essid;
		ParseResult& operator+=(const ParseResult& res);
	};

	std::map<MacAddress, ParseResult> resultMap;

	void parseSubFrame(const uint8_t* frame, size_t length, ParseResult& result);
	void parseBeaconFrame(const uint8_t* frame, size_t length, ParseResult& result);
public:
	void parse80211(const uint8_t* frame802, size_t length);
	void print();
};

