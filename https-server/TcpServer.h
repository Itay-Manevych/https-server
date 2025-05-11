#pragma once
#ifndef TCP_SERVER_INCLUDED
#define TCP_SERVER_INCLUDED

#include <WinSock2.h>
#include <iostream>
#include <string>

namespace http {
	class TcpServer {
		public:
			TcpServer(const std::string ip_address, const int port);
			~TcpServer();
		private:
			void exitWithError(const std::string& message);
			void log(const std::string& message);
			void StartServer();
			void CleanServer();

			WSADATA m_wsaData;
			SOCKET m_socket;
			std::string m_ipaddress;
			int m_port;
	};
}

#endif // !TCP_SERVER_INCLUDED
