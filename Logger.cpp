#include "Logger.hpp"
#include <iostream>
#include "Packet.hpp"


Logger * Logger::logger_instance_ = nullptr;

Logger::Logger():
	running_(true)
{
	logging_thread_ = std::thread(&Logger::writeData, this);
	out_.open("logs.txt", std::ios::app);
}

void Logger::writeData()
{
	while (running_)
	{		
		std::unique_lock<std::mutex> lock(mutex_);
		buffer_is_not_empty_.wait(lock, [this]() { return !packets_.empty(); });
		
		size_t size_of_commands_buffer = 0;
		for (Packet &packet : packets_)
		{
			size_of_commands_buffer += packet.command_string_.size() + 4; // 4 for " | " and '/n'
		}
		std::string commands_buffer;
		commands_buffer.reserve(size_of_commands_buffer);

		while (!packets_.empty())
		{
			commands_buffer += packets_.front().command_string_ + " | " + packets_.front().query_ + "\n";
			packets_.erase(packets_.begin());
		}

		lock.unlock();

		out_ << commands_buffer;

	}
}

Logger * Logger::getInstance()
{
	if (logger_instance_ == nullptr)
	{
		return new Logger();
	}

	return logger_instance_;
}

void Logger::log(std::vector<unsigned char> data_for_logging)
{
	std::unique_lock<std::mutex> lock(mutex_);

	if (!packets_.empty() && !packets_.back().is_ready_) {
		parser_.parse(packets_.back(), data_for_logging);
	}
	else {
		Packet new_packet;
		parser_.parse(new_packet, data_for_logging);
		packets_.push_back(new_packet);
	}
	if (packets_.back().is_ready_) {
		buffer_is_not_empty_.notify_one();
	}
}

Logger::~Logger()
{
	out_.close();
	running_ = false;
	logging_thread_.join();

}
