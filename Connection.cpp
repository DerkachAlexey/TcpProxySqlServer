
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>

#include "Logger.hpp"
#include "Connection.hpp"

Connection::Connection(io_service & io_service):
	client_socket_(io_service),
	server_socket_(io_service),
	data_to_client_(8192),
	data_to_server_(8192)

{

}

ip::tcp::socket & Connection::clientSocket()
{
	return client_socket_;
}

ip::tcp::socket & Connection::serverSocket()
{
	return server_socket_;
}

void Connection::start(std::string & remote_server_address, unsigned short remote_server_port)
{
	auto self_connection = shared_from_this();

	//connect to the server
	server_socket_.async_connect(
		ip::tcp::endpoint(ip::address::from_string(remote_server_address), remote_server_port),
		[self_connection](const boost::system::error_code & error_code)
	{
		self_connection->handleConnectToServer(error_code);
	}
	);
}

void Connection::closeConnection()
{
	std::lock_guard<std::mutex> lock(connecntion_mutex_);
	if (client_socket_.is_open())
	{
		client_socket_.close();
	}
	if (server_socket_.is_open())
	{
		server_socket_.close();
	}
}

void Connection::handleConnectToServer(const boost::system::error_code & error_code)
{
	if (error_code)
	{
		closeConnection();
		std::cerr << error_code.message()<<std::endl;

	}
	else
	{
		auto self_connection = shared_from_this();

		server_socket_.async_read_some(
			buffer(data_to_server_),
			[self_connection](const boost::system::error_code & error_code, size_t bytes_transferred)
		{
			self_connection->handleReadFromServer(error_code, bytes_transferred);
		}
		);

		client_socket_.async_read_some(
			buffer(data_to_client_),
			[self_connection](const boost::system::error_code & error_code, size_t bytes_transferred)
		{
			self_connection->handleReadFromClient(error_code, bytes_transferred);
		}
		);
	}
}

void Connection::handleReadFromServer(const boost::system::error_code & error_code, const size_t & bytes_transferred)
{
	if (!error_code)
	{

		auto self_connection = shared_from_this();

		async_write(client_socket_,
			boost::asio::buffer(data_to_server_, bytes_transferred),
			[self_connection](const boost::system::error_code& error_code, size_t size)
		{
			self_connection->handleWriteToClient(error_code);
		});
		
	}
	else
	{
		closeConnection();	
	}
		
}

void Connection::handleReadFromClient(const boost::system::error_code & error_code, const size_t & bytes_transferred)
{
	if (error_code)
	{
		closeConnection();

	}
	else
	{
		Logger::getInstance()->log(data_to_client_);

		auto self_connection = shared_from_this();

		async_write(server_socket_,
			buffer(data_to_client_, bytes_transferred),
			[self_connection](const boost::system::error_code & error_code, size_t size)
		{
			self_connection->handleWriteToServer(error_code);
		}
		);

	
	}

}

void Connection::handleWriteToClient(const boost::system::error_code & error_code)
{
	if (error_code)
	{
		closeConnection();

	}
	else
	{
		auto self_connection = shared_from_this();

		server_socket_.async_read_some(
			boost::asio::buffer(data_to_server_),
			[self_connection](const boost::system::error_code& error_code, size_t bytes_transferred)
		{
			self_connection->handleReadFromServer(error_code, bytes_transferred);
		});

		
	}
}

void Connection::handleWriteToServer(const boost::system::error_code & error_code)
{
	if (error_code)
	{
		closeConnection();

	}
	else
	{
		auto self_connection = shared_from_this();

		client_socket_.async_read_some(
			buffer(data_to_client_),
			[self_connection](const boost::system::error_code & error_code, size_t bytes_transferred)
		{
			self_connection->handleReadFromClient(error_code, bytes_transferred);
		}
		);

		
	}
}
