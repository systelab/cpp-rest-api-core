#include <iostream>
#include <stdexcept>

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Router/IRoute.h"
#include "RESTAPICore/Router/Router.h"
#include "RESTAPICore/Router/RoutesFactory.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"


class MyEndpoint : public systelab::rest_api_core::IEndpoint
{
public:
    MyEndpoint() = default;

    std::unique_ptr<systelab::web_server::Reply> execute(const systelab::rest_api_core::EndpointRequestData&) override
    {
        auto reply = std::make_unique<systelab::web_server::Reply>();
        reply->setStatus(systelab::web_server::Reply::OK);
        reply->setContent("{ \"message\": \"This is the reply for MyEndpoint\"}");
        return reply;
    }
};

int main(int argc, char *argv[])
{
    std::string jwtKey = "HereGoesYourJWTSecretKey";
    auto routesFactory = std::make_unique<systelab::rest_api_core::RoutesFactory>(jwtKey);

    auto router = std::make_unique<systelab::rest_api_core::Router>();
    router->addRoute(routesFactory->buildRoute("GET", "/rest/api/test", {}, []() { return std::make_unique<MyEndpoint>(); }) );

    systelab::web_server::Request request;
    request.setMethod("GET");
    request.setURI("/rest/api/test");
	
	auto reply = router->process(request);
	std::cout << "Endpoint reply -> Status " << reply->getStatus() << ", Content:" << reply->getContent() << std::endl;

    return 0;
}
