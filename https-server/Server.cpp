#include "TcpServer.h"

using namespace http;

int main() {
	TcpServer server = TcpServer("0.0.0.0", 3000);
	return 0;
}