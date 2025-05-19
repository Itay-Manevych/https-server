#include "NonSupportedRequestHandler.h"

void NonSupportedRequestHandler::HandleRequest(SOCKET client_socket, const std::string& request)
{
	std::string error_response = "This method has not been implemented, or is unkown!";

	std::string header = "HTTP/1.1 501 Not Implemented\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
		"\r\n";
	
	SendResponse(client_socket, header, error_response);
}