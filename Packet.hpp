#pragma once

#include <string>

class Packet
{
public:
	std::string query_;
	std::string command_string_;
	uint64_t query_length_ = 0u;
	unsigned char sequence_id_;
	unsigned char command_;
	bool is_ready_ = false;
	size_t current_index_ = 0;
};