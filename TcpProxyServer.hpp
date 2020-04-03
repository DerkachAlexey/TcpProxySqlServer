#pragma once
#include <boost/asio.hpp>

using namespace boost::asio;

class TcpProxyServer {
private:

	io_service & io_service_;
	ip::address local_host_ip_address_;
	unsigned short local_port_number_;
	std::shared_ptr<class Connection> connection_ptr_;
	std::string remote_server_address_;
	unsigned short remote_server_port_number_;
	ip::tcp::acceptor acceptor_;

public:

	TcpProxyServer(io_service & io_service, std::string & local_host_address, unsigned short local_port_number,
		std::string & remote_host_address, unsigned short remote_port_number);
	bool acceptConnections();
private:
	void handleAccept(const boost::system::error_code & error_code);

};