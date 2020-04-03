#include <boost/asio.hpp>
#include "TcpProxyServer.hpp"
#include <locale>
#include <iomanip>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUSSIAN");

	boost::asio::io_service ios;
	std::string local_host_address = "127.0.0.1";
	unsigned short local_port = 3307;

	std::string server_host_address = "127.0.0.1";
	unsigned short server_port = 3306;

	TcpProxyServer server(ios, local_host_address, local_port, local_host_address, server_port);
	server.acceptConnections();


	ios.run();

}