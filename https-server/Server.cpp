#include "TcpServer.h"

using namespace http;

int main() {
	TcpServer server = TcpServer("127.0.0.1", 8080);
	return 0;
}