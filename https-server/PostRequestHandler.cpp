#include "PostRequestHandler.h"
#include <filesystem>
#include <fstream>

void PostRequestHandler::HandleRequest(SOCKET client_socket, const std::string& request)
{
	std::string file_path = ExtractURI(request) + ".txt";

	if (std::filesystem::exists(file_path)) {
		std::string error_response = "File: " + file_path + "Already exists!";

		std::string header = "HTTP/1.1 409 Conflict \r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
			"\r\n";

		SendResponse(client_socket, header, error_response);
		return;
	}

	size_t pos = request.find("\r\n\r\n"); // start position of the body
	if (pos == std::string::npos) {
		std::string error_response = "Bad Request: There is no body to the request!";

		std::string header = "HTTP/1.1 400 Bad Request \r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
			"\r\n";

		SendResponse(client_socket, header, error_response);
		return;
	}

	// TODO: create file using the data of the body
}
