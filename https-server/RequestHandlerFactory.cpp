#include "RequestHandlerFactory.h"

std::unique_ptr<BaseRequestHandler> RequestHandlerFactory::CreateHandler(const std::string& request)
{
    std::unique_ptr<BaseRequestHandler> ptr_handler;
    if (request.find("GET") == 0) {
        return std::make_unique<GetRequestHandler>();
    }
    else if (request.find("POST") == 0) {
        return std::make_unique<PostRequestHandler>();
    }

    return std::make_unique<NonSupportedRequestHandler>();
}
