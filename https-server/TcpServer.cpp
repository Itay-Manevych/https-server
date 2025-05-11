#include "TcpServer.h"
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

// link with ws_32 in order to use WSAGetLastError()
#pragma comment(lib,"Ws2_32.lib")

using namespace http;

TcpServer::TcpServer()
{
	StartServer();
}

TcpServer::~TcpServer()
{
	CleanServer();
}

void TcpServer::exitWithError(const std::string& message)
{
	std::cout << message;
	exit(EXIT_FAILURE);
}

void TcpServer::log(const std::string& message)
{
	std::cout << message << std::endl;
}

void TcpServer::StartServer()
{
	WORD wVersion = MAKEWORD(2, 0);
	int startup = WSAStartup(wVersion, &m_wsaData);
	if (startup != 0) {
		exitWithError("WSAStartup failed. Error: " + std::to_string(WSAGetLastError()) + '\n');
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		exitWithError("Could not create socket. Error: " + std::to_string(WSAGetLastError()) + '\n');
	}
	m_socket = sock;
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

