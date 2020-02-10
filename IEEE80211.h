#pragma once
#include <cstdint>
#include "MacAddress.h"

#pragma pack(1)

struct Radiotap {
	uint8_t        it_version;
	uint8_t        it_pad;
	uint16_t       it_len;
	uint32_t       it_present;
};

namespace RadiotapFields {
	struct undefined {};

	// 0
	typedef struct TSFT {
		uint64_t machine;
	} _0;

	// 1
	typedef struct Flags {
		uint8_t flags;
	} _1;

	// 2
	typedef struct Rate {
		uint8_t rate;
	} _2;

	// 3
	typedef struct Channel {
		uint16_t frequency;
		uint16_t flags;
	} _3;

	// 4
	typedef struct FHSS {
		uint8_t hop_set;
		uint8_t hop_pattern;
	} _4;

	// 5
	typedef struct Antenna_signal {
		int8_t dBm;
	} _5;

	// 6
	typedef struct  Antenna_noise {
		int8_t dBm;
	} _6;

	// 7
	typedef struct Lock_quaility {
		uint16_t quality;
	} _7;

	// 8
	typedef struct TX_attenuation {
		uint16_t attenuation;
	} _8;

	// 9
	typedef struct db_TX_attenuation {
		uint16_t dB;
	} _9;

	// 10
	typedef struct dBm_TX_power {
		int8_t dBm;
	} _10;


	// 11
	typedef struct Antenna {
		uint8_t antenna_index;
	} _11;

	// 12
	typedef struct dB_antenna_signal {
		uint8_t dB;
	} _12;

	// 13
	typedef struct dB_antenna_noise {
		uint8_t dB;
	} _13;

	// 14
	typedef struct RX_flags {
		uint16_t flags;
	} _14;

	// 15 ~ 18 : Undefined
	typedef struct undefined _15;
	typedef struct undefined _16;
	typedef struct undefined _17;
	typedef struct undefined _18;

	// 19
	typedef struct MCS {
		uint8_t known;
		uint8_t flags;
		uint8_t mcs;
	} _19;

	// 20
	typedef struct A_MPDU_status {
		uint32_t reference_number;
		uint16_t flags;
		uint8_t delimiter_CRC_value;
		uint8_t reserved;
	} _20;

	// 21
	typedef struct VHT {
		uint16_t known;
		uint8_t flags;
		uint8_t bandwidth;
		uint8_t mcs_nss[4];
		uint8_t coding;
		uint8_t group_id;
		uint16_t partial_aid;
	} _21;

	// 22
	typedef struct timestamp {
		uint64_t timestamp;
		uint16_t accuracy;
		uint8_t unit_position;
		uint8_t flags;
	} _22;

	// 23
	typedef struct HE {
		uint16_t data1;
		uint16_t data2;
		uint16_t data3;
		uint16_t data4;
		uint16_t data5;
		uint16_t data6;
	} _23;

	// 24
	typedef struct HE_MU {
		uint16_t flags1;
		uint16_t flags2;
		uint8_t RU_channel1[4];
		uint8_t RU_channel2[4];
	} _24;

	// 25
	typedef struct HE_MU_other_user {
		uint16_t per_user_1;
		uint16_t per_user_2;
		uint8_t per_user_position;
		uint8_t per_user_known;
	} _25;

	// 26
	typedef struct Zero_length_psdu {
		uint8_t type;
	} _26;

	// 27
	typedef struct L_SIG {
		uint16_t data1;
		uint16_t data2;
	} _27;

	// 28
	typedef struct TLV_fields_in_radiotap {
		//??
	} _28;

	// 29
	typedef struct Radiotap_Namespace {
		//??
	} _29;

	// 30
	typedef struct Vendor_Namespace {
		//??
	} _30;

	// 31 : Undefined
	typedef struct undefined _31;

	// Size of Fields
	constexpr static size_t field_sizes[] = {
		sizeof(_0), sizeof(_1), sizeof(_2), sizeof(_3), sizeof(_4),
		sizeof(_5), sizeof(_6), sizeof(_7), sizeof(_8), sizeof(_9),
		sizeof(_10), sizeof(_11), sizeof(_12), sizeof(_13), sizeof(_14),
		sizeof(_15), sizeof(_16), sizeof(_17), sizeof(_18), sizeof(_19),
		sizeof(_20), sizeof(_21), sizeof(_22), sizeof(_23), sizeof(_24),
		sizeof(_25), sizeof(_26), sizeof(_27), sizeof(_28), sizeof(_29),
		sizeof(_30), sizeof(_31)
	};
}

struct FrameType {
	uint8_t version : 2;
	uint8_t type : 2;
	uint8_t subtype : 4;
	uint8_t flags;
};

struct BeaconFrame {
	uint16_t duration;
	MacAddress receiver_address;
	MacAddress transmitter_address;
	MacAddress bssid;
	uint16_t frag_seq_num;
	struct {
		uint64_t timestamp;
		uint16_t beacon_interval;
		uint16_t capabilities_information;
	} wireless_management;

	struct Tag {
		uint8_t num;
		uint8_t length;
	};
};

typedef BeaconFrame::Tag BeaconTag;

struct RSN_Information {
	uint16_t version;
	struct Cipher_Selector {
		uint8_t oui[3];
		uint8_t suite_type;
	} grp_cipher;
	uint16_t pairwise_cnt;
};

typedef RSN_Information::Cipher_Selector Cipher_Selector;

#pragma pack()