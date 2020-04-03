#include "TcpProxyServer.hpp"
#include "Connection.hpp"
#include <iostream>

TcpProxyServer::TcpProxyServer(io_service & io_service, std::string & local_host_address, unsigned short local_port_number,
	std::string & remote_host_address, unsigned short remote_port_number) :
	io_service_(io_service),
	local_host_ip_address_(ip::address_v4::from_string(local_host_address)), 
	local_port_number_(local_port_number),
	remote_server_address_(remote_host_address),
	remote_server_port_number_(remote_port_number),
	acceptor_(io_service_, ip::tcp::endpoint(local_host_ip_address_, local_port_number_))
{
}

bool TcpProxyServer::acceptConnections()
{
	try {
		connection_ptr_ = std::shared_ptr<Connection>(new Connection(io_service_));

		acceptor_.async_accept(connection_ptr_->clientSocket(),
			[this](const boost::system::error_code & error_code)
		{
			handleAccept(error_code);
		}
			);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

void TcpProxyServer::handleAccept(const boost::system::error_code & error_code)
{
	if (!error_code)
	{
		std::cout << "connection accepted!" << std::endl;
		connection_ptr_->start(remote_server_address_, remote_server_port_number_);
		if (!acceptConnections())
		{
			std::cerr << "Failed to accept connection" << std::endl;
		}
	}
	else
	{
		std::cerr << error_code.message() << std::endl;
	}
}
