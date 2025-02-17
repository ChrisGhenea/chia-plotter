/*
 * phase2.h
 *
 *  Created on: May 29, 2021
 *      Author: mad
 */

#ifndef INCLUDE_CHIA_PHASE2_H_
#define INCLUDE_CHIA_PHASE2_H_

#include <chia/chia.h>
#include <chia/phase1.h>
#include <chia/DiskSort.h>
#include <chia/bitfield.hpp>

#include <array>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <cstring>


namespace phase2 {

struct entry_x {
	uint32_t key;
	uint32_t pos;
	uint16_t off;		// 10 bit
	
	static constexpr size_t disk_size = 10;
	
	void assign(const phase1::tmp_entry_x& entry) {
		pos = entry.pos;
		off = entry.off;
	}
	size_t read(const uint8_t* buf) {
		memcpy(&key, buf, 4);
		memcpy(&pos, buf + 4, 4);
		memcpy(&off, buf + 8, 2);
		return disk_size;
	}
	size_t write(uint8_t* buf) const {
		memcpy(buf, &key, 4);
		memcpy(buf + 4, &pos, 4);
		memcpy(buf + 8, &off, 2);
		return disk_size;
	}
};

typedef phase1::tmp_entry_1 entry_1;
typedef phase1::entry_7 entry_7;

template<typename T>
struct get_pos {
	uint64_t operator()(const T& entry) {
		return entry.pos;
	}
};

template<typename T>
struct set_sort_key {
	void operator()(T& entry, uint32_t key) {
		entry.key = key;
	}
};

template<>
struct set_sort_key<entry_7> {
	void operator()(entry_7& entry, uint32_t key) {
		// no sort key
	}
};

typedef DiskSort<entry_x, get_pos<entry_x>> DiskSortT;
typedef DiskSort<entry_7, get_pos<entry_7>> DiskSort7;		// dummy

struct ouput_t {
	std::shared_ptr<bitfield> bitfield_1;
};


} // phase2

#endif /* INCLUDE_CHIA_PHASE2_H_ */
