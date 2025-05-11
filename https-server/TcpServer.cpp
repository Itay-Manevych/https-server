#include "TcpServer.h"
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

// link with ws_32 in order to use WSAGetLastError()
#pragma comment(lib,"Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace http;

TcpServer::TcpServer(const std::string ip_address, const int port)
{
	StartServer(ip_address, port);
	StartListen();
}

TcpServer::~TcpServer()
{
	CleanServer();
}

void TcpServer::exitWithError(const std::string& error_message)
{
	std::cout << error_message << std::endl;
	exit(EXIT_FAILURE);
}

void TcpServer::log(const std::string& message)
{
	std::cout << message << std::endl;
}

void TcpServer::StartServer(const std::string ip_address, const int port)
{
	WORD wVersion = MAKEWORD(2, 0);
	int startup = WSAStartup(wVersion, &m_wsaData);
	if (startup != 0) {
		exitWithError("WSAStartup failed. Error: " + std::to_string(WSAGetLastError()));
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		exitWithError("Could not create socket. Error: " + std::to_string(WSAGetLastError()));
	}
	m_socket = sock;

	m_socket_address.sin_family = AF_INET;
	m_socket_address.sin_port = htons(port);
	int result = InetPtonA(AF_INET, ip_address.c_str(), &m_socket_address.sin_addr.s_addr);
	if (result != 1) {
		exitWithError("Ip address is incorrect. Error: " + std::to_string(WSAGetLastError()));
	}

	int bind_result = bind(m_socket, (sockaddr*)&m_socket_address, sizeof(m_socket_address));
	if (bind_result == SOCKET_ERROR) {
		exitWithError("Could not bind to socket. Error: " + std::to_string(WSAGetLastError()));
	}
}

void TcpServer::CleanServer()
{
	int result = closesocket(m_socket);
	if (result == SOCKET_ERROR) {
		log("Failed to close socket. Error: " + std::to_string(WSAGetLastError()));
	}

	int cleanup_result = WSACleanup();
	if (cleanup_result != 0) {
		log("WSACleanup failed. Error: " + std::to_string(WSAGetLastError()));
	}

	if (result == SOCKET_ERROR || cleanup_result != 0) {
		exitWithError("An error occured when trying to clean server.");
	}

	log("Server cleaned succesfully");
}

void TcpServer::StartListen()
{
	int result = listen(m_socket, 20);
	if (result == SOCKET_ERROR) {
		exitWithError("Could not listen to socket. Error: " + std::to_string(WSAGetLastError()));
	}

	char ip_string[INET_ADDRSTRLEN]; // Length that is enough for ipv4
	const char* ip_result = InetNtopA(AF_INET, &m_socket_address.sin_addr, ip_string, sizeof(ip_string));
	if (!ip_result) {
		log("Failed to convert binary IP address to string. Error: " + std::to_string(WSAGetLastError()));
		return;
	}

	std::ostringstream ss;
	ss << "*** Server is listening on IP: " << ip_string
		<< " Port: " << ntohs(m_socket_address.sin_port) << " ***";
	log(ss.str());
}

