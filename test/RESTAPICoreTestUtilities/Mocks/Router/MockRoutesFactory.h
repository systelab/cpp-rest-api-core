#pragma once

#include "RESTAPICore/Router/IRoutesFactory.h"

#include "RESTAPICore/Router/IRoute.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockRoutesFactory : public IRoutesFactory
	{
	public:
		MockRoutesFactory();
		virtual ~MockRoutesFactory();

		MOCK_CONST_METHOD4(buildRouteProxy, IRoute* (const std::string&, const std::string&, const std::vector<RouteAccessValidatorFactoryMethod>&, EndpointFactoryMethod));
		std::unique_ptr<IRoute> buildRoute(const std::string& method,
										   const std::string& uri,
										   const std::vector<RouteAccessValidatorFactoryMethod>& accessValidatorFactoryMethods,
										   EndpointFactoryMethod endpointFactoryMethod) const
		{
			return std::unique_ptr<IRoute>(buildRouteProxy(method, uri, accessValidatorFactoryMethods, endpointFactoryMethod));
		}
	};

}}}

