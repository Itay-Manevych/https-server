#pragma once
#include <string>
#include <WinSock2.h>

class BaseRequestHandler {
	protected:
		void SendResponse(SOCKET client_socket, const std::string& header, const std::string& response);
		const std::string ExtractURI(const std::string& request);
	public:
		virtual void HandleRequest(SOCKET client_socket, const std::string& request) = 0;
};