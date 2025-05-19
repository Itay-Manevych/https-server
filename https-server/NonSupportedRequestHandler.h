#pragma once
#include "BaseRequestHandler.h"

class NonSupportedRequestHandler : public BaseRequestHandler {
	public:
		virtual void HandleRequest(SOCKET client_socket, const std::string& request) override final;
};