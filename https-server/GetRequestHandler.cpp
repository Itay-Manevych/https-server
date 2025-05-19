#include "GetRequestHandler.h"
#include <filesystem>
#include <fstream>

void GetRequestHandler::HandleRequest(SOCKET client_socket, const std::string& request)
{
	std::string file_path = ExtractURI(request) + ".txt";

	// file does not exist here
	if (!std::filesystem::exists(file_path)) {
		std::string error_response = "File: " + file_path + " does not exist!";

		std::string header = "HTTP/1.1 404 Not Found\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
			"\r\n";

		SendResponse(client_socket, header, error_response);
		closesocket(client_socket);
		return;
	}

	std::ifstream file(file_path);

	// problem opening file
	if (!file.is_open()) {
		std::string error_response = "Failed to open file: " + file_path;

		std::string header = "HTTP/1.1 500 Internal Server Error\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: " + std::to_string(error_response.size()) + "\r\n"
			"\r\n";


		SendResponse(client_socket, header, error_response);
		closesocket(client_socket);
		return;
	}

	std::string line;
	std::string response(file_path + " found succesfully! it's content is:\n");
	while (getline(file, line)) {
		response += line + "\n";
	}

	std::string header = "HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: " + std::to_string(response.size()) + "\r\n"
		"Connection: keep-alive\r\n"
		"\r\n";
	SendResponse(client_socket, header, response);

	file.close();
}
