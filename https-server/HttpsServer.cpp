#include "HttpsServer.h"
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

// link with ws_32 in order to use WSAGetLastError()
#pragma comment(lib,"Ws2_32.lib")
constexpr int NUM_OF_THREADS = 4;

using namespace http;

HttpsServer::HttpsServer(const std::string& ip_address, const int port)
{
	SetupServer(ip_address, port);
}

HttpsServer::~HttpsServer()
{
	CleanServer();
}

void HttpsServer::StartServer()
{
	CreateThreadPool(NUM_OF_THREADS);

	while (true) {
		AcceptClient();
	}
}

void HttpsServer::CreateThreadPool(int num_of_threads)
{
	for (int i = 0; i < num_of_threads; i++) {
		std::thread([this]() {
			while (true) {
				if (m_stop) {
					break;
				}

				// used in order to prevent race condition
				// that multiple threads will use the queue at the same time
				std::unique_lock<std::mutex> lock(m_queue_mutex); 
			
				// wait, and continue only if queue is not empty
				m_condition.wait(lock, [this]() {
					return !m_client_queue.empty() || m_stop;
				});

				// get and remove first client
				SOCKET client = m_client_queue.front();
				m_client_queue.pop();

				// unlock the lock and thus other threads can now use the queue 
				// while we are handling the current client
				lock.unlock();
				HandleClient(client);
			}
		}).detach(); // detach it thus making it run independently
	}
}

void HttpsServer::exitWithError(std::string& error_message, const bool should_show_error = false)
{
	if (should_show_error) {
		error_message += " Error:" + std::to_string(WSAGetLastError());
	}

	log(error_message);
	exit(EXIT_FAILURE);
}

void HttpsServer::log(const std::string& message)
{
	std::cout << message << std::endl;
}

void HttpsServer::SetupServer(const std::string& ip_address, const int port)
{
	// startup wsa
	WORD wVersion = MAKEWORD(2, 0);
	int startup = WSAStartup(wVersion, &m_wsaData);
	if (startup != 0) {
		std::string error_message = "WSAStartup failed.";
		exitWithError(error_message, true);
	}

	// create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::string error_message = "Could not create socket.";
		exitWithError(error_message, true);
	}
	m_socket = sock;

	// bind the socket
	m_socket_address.sin_family = AF_INET;
	m_socket_address.sin_port = htons(port);
	int result = InetPtonA(AF_INET, ip_address.c_str(), &m_socket_address.sin_addr.s_addr);
	if (result != 1) {
		std::string error_message = "Ip address is incorrect.";
		exitWithError(error_message, true);
	}

	result = bind(m_socket, (sockaddr*)&m_socket_address, sizeof(m_socket_address));
	if (result == SOCKET_ERROR) {
		std::string error_message = "Could not bind to socket.";
		exitWithError(error_message, true);
	}

	// start listening
	result = listen(m_socket, 20);
	if (result == SOCKET_ERROR) {
		std::string error_message = "Could not listen to socket. Error: " + std::to_string(WSAGetLastError());
		exitWithError(error_message);
	}

	char ip_string[INET_ADDRSTRLEN]; // Length that is enough for ipv4
	const char* ip_result = InetNtopA(AF_INET, &m_socket_address.sin_addr, ip_string, sizeof(ip_string));
	if (!ip_result) {
		std::string error_message = "Failed to convert binary IP address to string. Error: " + std::to_string(WSAGetLastError());
		exitWithError(error_message);
	}

	std::ostringstream ss;
	ss << "*** Server is listening on IP: " << ip_string
		<< " Port: " << ntohs(m_socket_address.sin_port) << " ***";
	log(ss.str());
}

void HttpsServer::CleanServer()
{
	m_stop = true;
	m_condition.notify_all();
	int result = closesocket(m_socket);
	if (result == SOCKET_ERROR) {
		log("Failed to close socket. Error: " + std::to_string(WSAGetLastError()));
	}

	int cleanup_result = WSACleanup();
	if (cleanup_result != 0) {
		log("WSACleanup failed. Error: " + std::to_string(WSAGetLastError()));
	}

	if (result == SOCKET_ERROR || cleanup_result != 0) {
		std::string error_message = "An error occured when trying to clean server.";
		exitWithError(error_message, false);
	}

	log("Server cleaned succesfully");
}

void HttpsServer::AcceptClient()
{
	sockaddr_in client_socket_address;
	int client_socket_address_size = sizeof(client_socket_address);

	SOCKET client_socket = accept(m_socket, (sockaddr*)&client_socket_address, &client_socket_address_size);
	if (client_socket == INVALID_SOCKET) {
		std::string error_message = "Could not connect to client socket.";
		exitWithError(error_message, true);
	}

	log("Client connected succesfully!");

	std::unique_lock<std::mutex> lock(m_queue_mutex);
	m_client_queue.push(client_socket);
	m_condition.notify_one();
}

void HttpsServer::HandleClient(SOCKET client_socket)
{
	char buffer[4096];
	int number_of_bytes = recv(client_socket, buffer, 4096, 0);

	// add a null terminator
	buffer[number_of_bytes] = '\0';

	for (int i = 0; i < number_of_bytes; i++) {
		std::cout << buffer[i];
	}
}


