#include "UnknownRequestHandler.h"

void UnkownRequestHandler::HandleRequest(SOCKET client_socket, const std::string& request)
{
	std::string error_response = "Unkown Method!";

	std::string header = "HTTP/1.1 501 Not Implemented\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
		"\r\n";
	
	SendResponse(client_socket, header, error_response);
}