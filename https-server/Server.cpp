#include "HttpsServer.h"

using namespace http;

int main() {
	HttpsServer server("127.0.0.1", 8080);
	server.StartServer();
	return 0;
}