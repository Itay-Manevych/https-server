#include "BaseRequestHandler.h"

const std::string BaseRequestHandler::ExtractURI(const std::string& request)
{
	size_t start = request.find(" ") + 2;
	size_t end = request.find(" ", start);
	int target_length = end - start;
	return request.substr(start, target_length);
}

void BaseRequestHandler::SendResponse(SOCKET client_socket, const std::string& header, const std::string& response)
{
	// Send the response header
	send(client_socket, header.c_str(), header.size(), 0);

	// Send the response content
	send(client_socket, response.c_str(), response.size(), 0);
}
