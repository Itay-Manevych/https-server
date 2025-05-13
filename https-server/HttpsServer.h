#pragma once
#ifndef HTTP_SERVER_INCLUDED
#define HTTP_SERVER_INCLUDED

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <queue>
#include <mutex>

namespace http {
	class HttpsServer {
		public:
			HttpsServer(const std::string& ip_address, const int port);
			~HttpsServer();
			void StartServer();

		private:
			void exitWithError(std::string& error_messagel, bool should_show_error);
			void log(const std::string& message);
			void SetupServer(const std::string& ip_address, const int port);
			void CleanServer();
			void AcceptClient();
			void HandleClient(SOCKET client_socket);
			void CreateThreadPool(int num_of_threads);

			// used in order to create a connection between sockets
			WSADATA m_wsaData;
			SOCKET m_socket;
			struct sockaddr_in m_socket_address;

			// used for thread pool:
			std::queue<SOCKET> m_client_queue;
			std::mutex m_queue_mutex;
			std::condition_variable m_condition;
			bool m_stop = false;
	};
}

#endif // !HTTP_SERVER_INCLUDED
