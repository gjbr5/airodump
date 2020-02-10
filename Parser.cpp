#include "Parser.h"
#include <iostream>
#include <bitset>
#include <iomanip>

//BSSID              PWR  Beacons    #Data, #/s  CH  MB   ENC  CIPHER AUTH ESSID


void Parser::parse80211(const uint8_t* frame802, size_t length) {
	ParseResult result = { MacAddress(),0,0,0,0,0,0,0,0,0,std::string() };

	// Radiotap Header
	const Radiotap* rt = reinterpret_cast<const Radiotap*>(frame802);

	// Radiotap Fields
	std::bitset<32> present = rt->it_present;

	// Radiotap Field 3 - Channel
	const RadiotapFields::Channel* channel = nullptr;

	// Radiotap Field 5 - Antenna Signal
	const RadiotapFields::Antenna_signal* pwr = nullptr;

	size_t offset = sizeof(Radiotap);
	for (size_t i = 0; i < 32; i++) {
		if (present[i]) {
			if (i == 3)
				channel = reinterpret_cast<const RadiotapFields::Channel*>(frame802 + offset);
			else if (i == 5)
				pwr = reinterpret_cast<const RadiotapFields::Antenna_signal*>(frame802 + offset);
			offset += RadiotapFields::field_sizes[i];
		}
	}

	// Channel Number
	if (channel) {
		if (channel->frequency < 2415)	result.channel = 1;
		else if (channel->frequency < 2420)	result.channel = 2;
		else if (channel->frequency < 2425)	result.channel = 3;
		else if (channel->frequency < 2430)	result.channel = 4;
		else if (channel->frequency < 2435)	result.channel = 5;
		else if (channel->frequency < 2440)	result.channel = 6;
		else if (channel->frequency < 2445)	result.channel = 7;
		else if (channel->frequency < 2450)	result.channel = 8;
		else if (channel->frequency < 2455)	result.channel = 9;
		else if (channel->frequency < 2460)	result.channel = 10;
		else if (channel->frequency < 2465)	result.channel = 11;
		else if (channel->frequency < 2470)	result.channel = 12;
		else if (channel->frequency < 2479)	result.channel = 13;
		else result.channel = 14;
	}

	if (pwr)
		result.pwr = pwr->dBm;

	if (length > rt->it_len)
		parseSubFrame(frame802 + rt->it_len, length - rt->it_len, result);

	std::pair<decltype(resultMap)::iterator, bool> empRes = resultMap.emplace(result.bssid, result);
	if (!empRes.second)
		empRes.first->second += result;
}

void Parser::parseSubFrame(const uint8_t* frame, size_t length, ParseResult& result) {
	// Type
	const FrameType* type = reinterpret_cast<const FrameType*>(frame);
	if (type->type == 0 && type->subtype == 8) { // Beacon Frame
		parseBeaconFrame(frame + sizeof(FrameType), length - sizeof(FrameType), result);
	}
	else if (type->type == 0 && type->subtype == 5) { // Data Frame 
		result.data++;
	}
}

void Parser::parseBeaconFrame(const uint8_t* frame, size_t length, ParseResult& result) {
	size_t offset = 0;
	result.beacons++;

	// Beacon Frame
	const BeaconFrame* bf = reinterpret_cast<const BeaconFrame*>(frame);
	offset += sizeof(BeaconFrame);

	result.bssid = bf->bssid;

	if (length <= offset)
		return;

	// Parse Tag
	while (length > offset) {
		const BeaconTag* tag = reinterpret_cast<const BeaconTag*>(frame + offset);
		offset += sizeof(BeaconTag) + tag->length;
		switch (tag->num) {
		case 0: // SSID
		{
			char* ssid = new char[static_cast<size_t>(tag->length) + 1]();
			for (size_t i = 0; i < tag->length; i++)
				ssid[i] = reinterpret_cast<const char*>(tag + 1)[i];
			result.essid = ssid;
			delete[] ssid;
			break;
		}
		case 48: // RSN Information
		{
			const struct RSN_Information* rsnInfo = reinterpret_cast<decltype(rsnInfo)>(tag + 1);
			if (std::memcmp(rsnInfo->grp_cipher.oui, "\x00\x0F\xAC", 3))
				break;
			result.cipher = rsnInfo->grp_cipher.suite_type;
			const uint16_t* akm_cnt = reinterpret_cast<const uint16_t*>(reinterpret_cast<const uint8_t*>(rsnInfo + 1) + sizeof(Cipher_Selector) * rsnInfo->pairwise_cnt);
			const Cipher_Selector* akm = reinterpret_cast<const Cipher_Selector*>(akm_cnt + 1);
			for (uint16_t i = 0; i < *akm_cnt; i++) {
				if (std::memcmp(akm->oui, "\x00\x0F\xAC", 3))
					continue;
				if (akm->suite_type == 1 || akm->suite_type == 2)
					result.auth = akm->suite_type;
				akm += 1;
			}
		}
		default:
			break;
		}
	}
}

void Parser::print() {
	std::cout << "BSSID              PWR  Beacons    #Data  #/s  CH  MB   ENC  CIPHER AUTH ESSID\n";
	for (auto& it : resultMap) {
		ParseResult& result = it.second;
		std::cout << result.bssid;
		std::cout << "  ";

		if (result.pwr) std::cout << static_cast<int>(result.pwr);
		else std::cout << "NULL";
		std::cout << "\t ";

		std::cout << std::setw(6) << std::setfill(' ') << result.beacons;

		std::cout << "\t   ";
		std::cout << std::setw(5) << std::setfill(' ') << result.data;

		std::cout << "       ";

		std::cout << std::setw(2) << std::setfill(' ') << result.channel;

		std::cout << "              ";

		switch (result.cipher) {
		case 2:
			std::cout << "TKIP";
			break;
		case 4:
			std::cout << "CCMP";
			break;
		case 1:
		case 5:
			std::cout << " WEP";
			break;
		default:
			std::cout << "    ";
			break;
		}
		std::cout << "  ";

		switch (result.auth) {
		case 1:
			std::cout << "MGT";
			break;
		case 2:
			std::cout << "PSK";
			break;
		default:
			std::cout << "   ";
			break;
		}
		std::cout << " ";

		std::cout << result.essid;
		std::cout << '\n';
	}
}


Parser::ParseResult& Parser::ParseResult::operator+=(const Parser::ParseResult& res) {
	if (bssid < res.bssid || res.bssid < bssid)
		return *this;
	pwr = res.pwr;
	beacons += res.beacons;
	data += res.data;
	per_second += res.per_second;
	channel = res.channel;
	mb = res.mb;
	enc = res.enc;
	cipher = res.cipher;
	auth = res.auth;
	essid = res.essid;
}