#include "stdafx.h"
#include "RESTAPICore/Router/RoutesFactory.h"
#include "RESTAPICore/Router/IRoute.h"

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"

#include "WebServerAdapterInterface/Model/Request.h"


using namespace testing;

namespace systelab { namespace rest_api_core { namespace unit_test {

	class RouteTest : public Test
	{
	public:
		void SetUp()
		{
			setUpRoutesFactory();
			setUpEndpointFactoryMethod();
		}
	
		void setUpRoutesFactory()
		{
			m_jwtKey = "ThisIsMyKey";
			m_routesFactory = std::make_unique<RoutesFactory>(m_jwtKey);
		}

		void setUpEndpointFactoryMethod()
		{
			m_endpointFactoryMethod =
				[this]() -> std::unique_ptr<IEndpoint>
				{
					auto endpoint = std::make_unique<test_utility::MockEndpoint>();
					ON_CALL(*endpoint, executeProxy(_)).WillByDefault(Invoke(
						[this](const EndpointRequestData&) -> web_server::Reply*
						{
							auto reply = std::make_unique<web_server::Reply>();
							reply->setStatus(web_server::Reply::OK);
							reply->setContent("{}");
							return reply.release();
						}
					));

					return endpoint;
				};
		}

		std::unique_ptr<web_server::Request> buildSimpleRequest(const std::string& method,
																const std::string& uri)
		{
			auto request = std::make_unique<web_server::Request>();
			request->setMethod(method);
			request->setURI(uri);

			return request;
		}

	protected:
		std::string m_jwtKey;
		std::unique_ptr<RoutesFactory> m_routesFactory;
		std::function< std::unique_ptr<IEndpoint>() > m_endpointFactoryMethod;
	};


	TEST_F(RouteTest, testPublicRouteReturnsExpectedReplyWhenMatchingRequest)
	{
		auto route = m_routesFactory->buildRoute("POST", "/rest/api/test", {}, m_endpointFactoryMethod);

		auto request = buildSimpleRequest("POST", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

}}}
