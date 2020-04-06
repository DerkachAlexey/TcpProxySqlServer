#pragma once

#include <mutex>
#include <condition_variable>
#include <vector>
#include <fstream>
#include <thread>
#include "PacketsParser.hpp"


class Logger
{
private:
	Logger();
	std::mutex mutex_;
	static Logger * logger_instance_;
	std::thread logging_thread_;
	std::condition_variable buffer_is_not_empty_;
	bool running_;
	std::vector<class Packet> packets_;
	std::ofstream out_;
	PacketParser parser_;

	void writeData();
public:
	Logger(Logger const & Logger) = delete;
	Logger& operator = (Logger  & Logger) = delete;
    static Logger * getInstance();
	void log(std::vector<unsigned char> data_for_logging);
	~Logger();

};