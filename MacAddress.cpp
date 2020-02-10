#include "MacAddress.h"
#include <memory.h>
#include <iomanip>

MacAddress::MacAddress() {
	std::memset(address, 0, ADDRLEN);
}

bool MacAddress::operator<(const MacAddress& o) const {
	return memcmp(address, o.address, MacAddress::ADDRLEN) < 0;
}
bool MacAddress::operator==(const MacAddress& o) const {
	return memcmp(address, o.address, MacAddress::ADDRLEN) == 0;
}

std::ostream& operator<<(std::ostream& os, const MacAddress address) {
	for (size_t i = 0; i < MacAddress::ADDRLEN - 1; i++)
		os << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(address.address[i]) << ":";
	os << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(address.address[MacAddress::ADDRLEN - 1]);
	os << std::dec;
	return os;
}