#include "PacketsParser.hpp"
#include "Packet.hpp"

void PacketParser::parse(Packet & query_packet, std::vector<unsigned char> query_data)
{
	for (unsigned char & byte : query_data)
	{
		switch (currentState) {
		case State::Payload_length_1: {
			query_packet.query_length_ |= static_cast<uint64_t>(byte);
			currentState = State::Payload_length_2;
			break;
		}
		case State::Payload_length_2: {
			query_packet.query_length_ |= static_cast<uint64_t>(byte << 8u);
			currentState = State::Payload_length_3;
			break;
		}
		case State::Payload_length_3: {
			query_packet.query_length_ |= static_cast<uint64_t>(byte << 16u);
			currentState = State::Sequence_id;
			break;
		}
		case State::Sequence_id: {
			query_packet.sequence_id_ = byte;
			currentState = State::Command;
			break;
		}
		case State::Command: {
			query_packet.command_ = byte;
			++query_packet.current_index_;
			switch (static_cast<Command>(query_packet.command_)) {
			case Command::COM_QUERY: {
				query_packet.command_string_ += "COM_QUERY";
				break;
			}
			case Command::COM_QUIT: {
				query_packet.command_string_ += "COM_QUIT";
				break;
			}
			case Command::COM_INIT_DB: {
				query_packet.command_string_ += "COM_INIT_DB";
				break;
			}
			case Command::COM_FIELD_LIST: {
				query_packet.command_string_ += "COM_FIELD_LIST";
				break;
			}
			case Command::COM_REFRESH: {
				query_packet.command_string_ += "COM_REFRESH";
				break;
			}
			case Command::COM_STATISTICS: {
				query_packet.command_string_ += "COM_STATISTICS";
				break;
			}
			case Command::COM_PROCESS_INFO: {
				query_packet.command_string_ += "COM_PROCESS_INFO";
				break;
			}
			case Command::COM_PROCESS_KILL: {
				query_packet.command_string_ += "COM_PROCESS_KILL";
				break;
			}
			case Command::COM_DEBUG: {
				query_packet.command_string_ += "COM_DEBUG";
				break;
			}
			case Command::COM_CHANGE_USER: {
				query_packet.command_string_ += "COM_CHANGE_USER";
				break;
			}
			case Command::COM_RESET_CONNECTION: {
				query_packet.command_string_ += "COM_RESET_CONNECTION";
				break;
			}
			case Command::COM_SET_OPTION_AND_STMT_RESET: {
				query_packet.command_string_ += "COM_SET_OPTION_OR_STMT_RESET";
				break;
			}
			case Command::COM_STMT_PREPARE: {
				query_packet.command_string_ += "COM_STMT_PREPARE";
				break;
			}
			case Command::COM_STMT_EXECUTE: {
				query_packet.command_string_ += "COM_STMT_EXECUTE";
				break;
			}
			case Command::COM_STMT_FETCH_AND_CLOSE: {
				query_packet.command_string_ += "COM_STMT_FETCH_OR_STMT_CLOSE";
				break;
			}
			case Command::COM_STMT_SEND_LONG_DATA: {
				query_packet.command_string_ += "COM_STMT_RESET";
				break;
			}
			default: {
				break;
			}
			}
			currentState = State::Query;
			break;
		}
		case State::Query: {
			query_packet.query_ += static_cast<char>(byte);
			++query_packet.current_index_;
			break;
		}
		}

		if (query_packet.current_index_ >= query_packet.query_length_) {
			currentState = State::Payload_length_1;
			query_packet.is_ready_ = true;
		}
	}
}
