#include "GetRequestHandler.h"
#pragma once
#include <memory>
#include "GetRequestHandler.h"
#include "PostRequestHandler.h"

class RequestHandlerFactory {
	public:
		std::unique_ptr<BaseRequestHandler> CreateHandler(const std::string& request);
};