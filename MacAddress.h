#pragma once
#include <cstdint>
#include <iostream>

typedef struct MacAddress
{
public:
	static constexpr size_t ADDRLEN = 6;
private:
	uint8_t address[ADDRLEN];
public:
	MacAddress();
	bool operator<(const MacAddress& o) const;
	bool operator==(const MacAddress& o) const;
	friend std::ostream& operator<<(std::ostream& os, const MacAddress address);
} MacAddress;
