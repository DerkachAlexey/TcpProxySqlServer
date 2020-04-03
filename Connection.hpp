#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <mutex>

using namespace boost::asio;

class Connection : public std::enable_shared_from_this<Connection>
{
private:
	ip::tcp::socket client_socket_;
	ip::tcp::socket server_socket_;
	std::vector<unsigned char> data_to_client_;
	std::vector<unsigned char> data_to_server_;
	std::mutex connecntion_mutex_;
public: 
	Connection(io_service & io_service);
	ip::tcp::socket & clientSocket();
	ip::tcp::socket & serverSocket();
	void start(std::string & remote_server_address, unsigned short remote_server_port);
	void closeConnection();
private:
	void handleConnectToServer(const boost::system::error_code & error_code);
	void handleReadFromServer(const boost::system::error_code & error_code, const size_t &bytes_transferred);
	void handleReadFromClient(const boost::system::error_code & error_code, const size_t &bytes_transferred);
	void handleWriteToClient(const boost::system::error_code & error_code);
	void handleWriteToServer(const boost::system::error_code & error_code);
};