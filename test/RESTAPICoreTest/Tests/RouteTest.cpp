#include "stdafx.h"
#include "RESTAPICore/Router/RoutesFactory.h"
#include "RESTAPICore/Router/IRoute.h"

#include "WebServerAdapterInterface/Model/Request.h"

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::rest_api_core::test_utility;

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
			ON_CALL(m_endpoint, executeProxy(_)).WillByDefault(ReturnNull());

			m_endpointFactoryMethod =
				[this]() -> std::unique_ptr<IEndpoint>
				{
					auto endpoint = std::make_unique<MockEndpoint>();
					ON_CALL(*endpoint, executeProxy(_)).WillByDefault(Invoke(
						[this](const EndpointRequestData& endpointRequestData) -> web_server::Reply*
						{
							auto reply = std::make_unique<web_server::Reply>();
							reply->setStatus(web_server::Reply::OK);
							reply->setContent("{}");

							m_endpoint.executeProxy(endpointRequestData);

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
		MockEndpoint m_endpoint;
	};


	// Basic routes
	TEST_F(RouteTest, testPublicRouteReturnsExpectedReplyWhenMatchingRequest)
	{
		EndpointRequestData expectedEndpointRequestData;
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testPublicRouteReturnsNullWhenMethodDoesNotMatch)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply == NULL);
	}

	TEST_F(RouteTest, testPublicRouteReturnsNullWhenFirstURIFragmentDoesNotMatch)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/my/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply == NULL);
	}

	TEST_F(RouteTest, testPublicRouteReturnsNullWhenLastURIFragmentDoesNotMatch)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/anothertest");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply == NULL);
	}

	TEST_F(RouteTest, testPublicRouteReturnsNullWhenDifferentNumberOfURIFragments)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/v2/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply == NULL);
	}


	// Routes with parameters
	TEST_F(RouteTest, testPublicRouteWithSingleNumericParameter)
	{
		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setParameters({ {}, { {"id", 23} } });
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/resource/+id", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/resource/23");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testPublicRouteWithMultipleNumericParameters)
	{
		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setParameters({ {}, { {"id", 54}, {"id2", 23} } });
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/resource/+id/subresource/+id2", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/resource/54/subresource/23");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testPublicRouteWithSingleStringParameter)
	{
		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setParameters({ { {"uuid", "C9C70401-807C-4B57-BE62-FC2723D4C43B"} }, {} });
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("POST", "/rest/api/resource/:uuid", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("POST", "/rest/api/resource/C9C70401-807C-4B57-BE62-FC2723D4C43B");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testPublicRouteWithMultipleStringParameters)
	{
		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setParameters({ { {"start", "peter"}, {"end", "joe"} }, {} });
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/:start/:end", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/peter/joe");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testPublicRouteWithNumericAndStringParameters)
	{
		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setParameters({ { {"param1", "one"}, {"param3", "three"} }, { {"param2", 2} } });
		EXPECT_CALL(m_endpoint, executeProxy(isEqualTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/:param1/item/+param2/item/:param3", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/one/item/2/item/three");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

}}}
