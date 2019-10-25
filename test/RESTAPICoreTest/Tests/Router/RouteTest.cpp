#include "stdafx.h"
#include "RESTAPICore/Router/RoutesFactory.h"
#include "RESTAPICore/Router/IRoute.h"

#include "WebServerAdapterInterface/Model/Headers.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"
#include "RESTAPICoreTestUtilities/Mocks/RouteAccess/MockRouteAccessValidator.h"

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
			setUpAccessValidatorFactoryMethods();
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

		void setUpAccessValidatorFactoryMethods()
		{
			m_passAccessValidatorFactoryMethod =
				[this]() -> std::unique_ptr<IRouteAccessValidator>
				{
					auto routeAccessValidator = std::make_unique<MockRouteAccessValidator>();
					ON_CALL(*routeAccessValidator, hasAccess(_)).WillByDefault(Return(true));
					return routeAccessValidator;
				};

			m_failAccessValidatorFactoryMethod =
				[this]() -> std::unique_ptr<IRouteAccessValidator>
				{
					auto routeAccessValidator = std::make_unique<MockRouteAccessValidator>();
					ON_CALL(*routeAccessValidator, hasAccess(_)).WillByDefault(Return(false));
					return routeAccessValidator;
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

		std::unique_ptr<web_server::Request> buildRequestWithHeaders(const std::string& method,
																	 const std::string& uri,
																	 const web_server::RequestHeaders& headers)
		{
			auto request = std::make_unique<web_server::Request>();
			request->setMethod(method);
			request->setURI(uri);

			for (auto headerPair : headers.getHeadersMap())
			{
				request->getHeaders().addHeader(headerPair.first, headerPair.second);
			}

			return request;
		}


	protected:
		std::string m_jwtKey;
		std::unique_ptr<RoutesFactory> m_routesFactory;
		std::function< std::unique_ptr<IEndpoint>() > m_endpointFactoryMethod;
		MockEndpoint m_endpoint;
		std::function< std::unique_ptr<IRouteAccessValidator>() > m_passAccessValidatorFactoryMethod;
		std::function< std::unique_ptr<IRouteAccessValidator>() > m_failAccessValidatorFactoryMethod;
	};


	// Basic routes
	TEST_F(RouteTest, testPublicRouteReturnsExpectedReplyWhenMatchingRequest)
	{
		EndpointRequestData expectedEndpointRequestData;
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

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
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

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
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

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
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

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
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

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
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/:param1/item/+param2/item/:param3", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/one/item/2/item/three");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testRequestWithURIWithStringParameterCodification)
	{
		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/:id");
		auto reply = route->execute(*request);
		ASSERT_TRUE(reply == NULL);
	}

	TEST_F(RouteTest, testRequestWithURIWithNumericParameterCodification)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/+id");
		auto reply = route->execute(*request);
		ASSERT_TRUE(reply == NULL);
	}


	// Headers & Authorization data
	TEST_F(RouteTest, testRequestWithAnotherHeaderCallsEndpointWithEmptyAuthorizationData)
	{
		web_server::RequestHeaders requestHeaders;
		requestHeaders.addHeader(web_server::CONTENT_TYPE, "application/json");

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setHeaders(requestHeaders);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildRequestWithHeaders("GET", "/rest/api/test", requestHeaders);
		route->execute(*request);
	}

	TEST_F(RouteTest, testRequestWithAuthorizationHeaderWithoutProtocolCallsEndpointWithEmptyAuthorizationData)
	{
		std::string authorizationHeader = "anNtaXRoOm15cGFzc3dvcmQ=";
		web_server::RequestHeaders requestHeaders;
		requestHeaders.addHeader(web_server::AUTHORIZATION, authorizationHeader);

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setHeaders(requestHeaders);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildRequestWithHeaders("GET", "/rest/api/test", requestHeaders);
		route->execute(*request);
	}

	TEST_F(RouteTest, testRequestWithAuthorizationHeaderForAnotherProtocolCallsEndpointWithEmptyAuthorizationData)
	{
		std::string authorizationHeader = "Basic anNtaXRoOm15cGFzc3dvcmQ=";
		web_server::RequestHeaders requestHeaders;
		requestHeaders.addHeader(web_server::AUTHORIZATION, authorizationHeader);

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setHeaders(requestHeaders);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildRequestWithHeaders("GET", "/rest/api/test", requestHeaders);
		route->execute(*request);
	}

	TEST_F(RouteTest, testRequestWithAuthorizationHeaderWithBearerAndInvalidDataCallsEndpointWithEmptyAuthorizationData)
	{
		std::string authorizationHeader = "Bearer anNtaXRoOm15cGFzc3dvcmQ=";
		web_server::RequestHeaders requestHeaders;
		requestHeaders.addHeader(web_server::AUTHORIZATION, authorizationHeader);

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setHeaders(requestHeaders);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildRequestWithHeaders("GET", "/rest/api/test", requestHeaders);
		route->execute(*request);
	}

	TEST_F(RouteTest, testRequestWithAuthorizationHeaderWithBearerAndValidDataCallsEndpointWithAuthorizationDataClaims)
	{
		std::string authorizationHeader = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
										  "eyJzdWIiOiJqc21pdGgifQ.0-GCEvX4THSns23xDx2awrWPGPOtvJcEkQbma4nGWZg";
		web_server::RequestHeaders requestHeaders;
		requestHeaders.addHeader(web_server::AUTHORIZATION, authorizationHeader);

		EndpointRequestAuthorizationClaims expectedAuthorizationClaims;
		expectedAuthorizationClaims.addClaim("sub", "jsmith");

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setHeaders(requestHeaders);
		expectedEndpointRequestData.setAuthorizationClaims(expectedAuthorizationClaims);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildRequestWithHeaders("GET", "/rest/api/test", requestHeaders);
		route->execute(*request);
	}


	// Query strings
	TEST_F(RouteTest, testRequestWithQueryStringsCallsEndpointWithSameQueryStrings)
	{
		web_server::RequestQueryStrings requestQueryStrings;
		requestQueryStrings.addItem("sort", "field");
		requestQueryStrings.addItem("page", "23");

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setQueryStrings(requestQueryStrings);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto request = buildSimpleRequest("GET", "/rest/api/test");
		for (auto queryStringItem : requestQueryStrings.getItemsMap())
		{
			request->getQueryStrings().addItem(queryStringItem.first, queryStringItem.second);
		}

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		route->execute(*request);
	}


	// Content
	TEST_F(RouteTest, testRequestWithContentCallsEndpointWithSameContent)
	{
		std::string requestContent = "This is a fancy request content";

		EndpointRequestData expectedEndpointRequestData;
		expectedEndpointRequestData.setContent(requestContent);
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto request = buildSimpleRequest("GET", "/rest/api/test");
		request->setContent(requestContent);

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		route->execute(*request);
	}


	// Access validators
	TEST_F(RouteTest, testRouteWithSingleAccessValidatorThatFailsReturnsForbiddenReply)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", { m_failAccessValidatorFactoryMethod }, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);
	
		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::FORBIDDEN, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testRouteWithSingleAccessValidatorThatPassesCallsEndpoint)
	{
		EndpointRequestData expectedEndpointRequestData;
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", { m_passAccessValidatorFactoryMethod }, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testRouteWithMultipleAccessValidatorsAndOneFailsReturnsForbiddenReply)
	{
		EXPECT_CALL(m_endpoint, executeProxy(_)).Times(0);

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test",
												 { m_passAccessValidatorFactoryMethod, m_failAccessValidatorFactoryMethod,
												   m_passAccessValidatorFactoryMethod }, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::FORBIDDEN, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}

	TEST_F(RouteTest, testRouteWithMultipleAccessValidatorsAndAllPassCallsEndpoint)
	{
		EndpointRequestData expectedEndpointRequestData;
		EXPECT_CALL(m_endpoint, executeProxy(isEqTo(expectedEndpointRequestData)));

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test",
												 { m_passAccessValidatorFactoryMethod, m_passAccessValidatorFactoryMethod,
												   m_passAccessValidatorFactoryMethod }, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::OK, reply->getStatus());
		ASSERT_EQ("{}", reply->getContent());
	}


	// Error handling
	TEST_F(RouteTest, testUnableToBuildEndpointReturnsInternalServerErrorReply)
	{
		m_endpointFactoryMethod =
			[this]() -> std::unique_ptr<IEndpoint>
			{
				return std::unique_ptr<IEndpoint>();
			};

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		ASSERT_EQ("{ \"reason\": \"Unable to build endpoint for GET /rest/api/test\"}", reply->getContent());
	}

	TEST_F(RouteTest, testEndpointExceptionReturnsInternalServerErrorReply)
	{
		m_endpointFactoryMethod =
			[this]() -> std::unique_ptr<IEndpoint>
			{
				auto endpoint = std::make_unique<MockEndpoint>();
				ON_CALL(*endpoint, executeProxy(_)).WillByDefault(Throw(std::runtime_error("Thrown error details")));
				return endpoint;
			};

		auto route = m_routesFactory->buildRoute("GET", "/rest/api/test", {}, m_endpointFactoryMethod);
		auto request = buildSimpleRequest("GET", "/rest/api/test");
		auto reply = route->execute(*request);

		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		ASSERT_EQ("{ \"reason\": \"Unknown error while processing endpoint "
				  "GET /rest/api/test: Thrown error details\"}", reply->getContent());
	}


}}}
