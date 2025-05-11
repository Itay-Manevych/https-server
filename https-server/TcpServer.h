#pragma once
#ifndef TCP_SERVER_INCLUDED
#define TCP_SERVER_INCLUDED

#include <WinSock2.h>
#include <iostream>

namespace http {
	class TcpServer {
		public:
			TcpServer();
			~TcpServer();
		private:
			void exitWithError(const std::string& message);
			void log(const std::string& message);
			void StartServer();
			void CleanServer();
			WSADATA m_wsaData;
			SOCKET m_socket;
	};
}

#endif // !TCP_SERVER_INCLUDED
