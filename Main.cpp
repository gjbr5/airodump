#include "Parser.h"
#include <pcap.h>

int main(int argc, char* argv[])
{
	const char* file = "C:\\Users\\gjbr5\\Desktop\\80211.pcapng";
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle = pcap_open_offline(file, errbuf);
	if (!handle) {
		std::cout << "File Open Failed.\n";
		return 0;
	}
	Parser parser;

	while (true) {
		struct pcap_pkthdr* header;
		const uint8_t* packet;
		if (pcap_next_ex(handle, &header, &packet) < 0) break;
		parser.parse80211(packet, header->caplen);
	}
	pcap_close(handle);

	parser.print();

}