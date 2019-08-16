#include "stdafx.h"

#include "RESTAPICore/Endpoint/ClaimConstants.h"
#include "RESTAPICore/RouteAccess/TokenExpirationAccessValidator.h"
#include "RESTAPICore/RouteAccess/UserRoleRouteAccessValidator.h"
#include "RESTAPICore/Router/IRoute.h"
#include "RESTAPICore/Router/Router.h"
#include "RESTAPICore/Router/RoutesFactory.h"

#include "RapidjsonAdapter/JSONAdapter.h"

#include "JWTUtils/Services/TokenBuilderService.h"

#include "WebServerAdapterInterface/Model/Headers.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"
#include "RESTAPICoreTestUtilities/Mocks/RouteAccess/MockUserRoleService.h"
#include "RESTAPICoreTestUtilities/Stubs/StubEpochTimeService.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"

#include "TestUtilitiesInterface/EntityComparator.h"

#include <chrono>


using namespace testing;
using namespace systelab::json::test_utility;
using namespace systelab::rest_api_core::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	namespace {
		enum class UserRole
		{
			ANONYMOUS = 0,
			BASIC = 1,
			ADMIN = 2
		};
	}

	class RouterComponentTest : public Test
	{
	public:
		void SetUp()
		{
			setUpJWTBuilderService();
			setUpRoutesFactory();
			setUpAccessValidatorFactoryMethods();
			setUpUserRoleService();

			m_router = std::make_unique<Router>();
			m_router->addRoute(buildRoute("GET",    "/rest/api/users",         UserRole::BASIC));
			m_router->addRoute(buildRoute("GET",    "/rest/api/users/+id",     UserRole::BASIC));
			m_router->addRoute(buildRoute("POST",   "/rest/api/users",         UserRole::ADMIN));
			m_router->addRoute(buildRoute("PUT",    "/rest/api/users/+id",     UserRole::ADMIN));
			m_router->addRoute(buildRoute("DELETE", "/rest/api/users/+id",     UserRole::ADMIN));
			m_router->addRoute(buildRoute("POST",   "/rest/api/users/login",   UserRole::ANONYMOUS));
		}

		void setUpJWTBuilderService()
		{
			m_jwtKey = "RouterComponentTestKey";
			m_tokenBuilderService = std::make_unique<jwt::TokenBuilderService>(m_jsonAdapter);
		}

		void setUpRoutesFactory()
		{
			m_routesFactory = std::make_unique<RoutesFactory>(m_jwtKey);
		}

		void setUpAccessValidatorFactoryMethods()
		{
			m_tokenExpirationAccessValidator =
				[this]() -> std::unique_ptr<IRouteAccessValidator>
				{
					long expirationSeconds = 600; // 10 minutes
					return std::make_unique<TokenExpirationAccessValidator>(expirationSeconds, m_epochTimeService);
				};

			m_adminUserAccessValidator =
				[this]() -> std::unique_ptr<IRouteAccessValidator>
				{
					std::vector<std::string> allowedRoles = { "Admin" };
					return std::make_unique<UserRoleRouteAccessValidator>(allowedRoles, m_userRoleService);
				};

			m_basicUserAccessValidator =
				[this]() -> std::unique_ptr<IRouteAccessValidator>
				{
					std::vector<std::string> allowedRoles = { "Admin", "Basic" };
					return std::make_unique<UserRoleRouteAccessValidator>(allowedRoles, m_userRoleService);
				};
		}

		void setUpUserRoleService()
		{
			ON_CALL(m_userRoleService, getUserRoles(_)).WillByDefault(Return(std::vector<std::string>()));

			m_adminUsername = "Batman";
			ON_CALL(m_userRoleService, getUserRoles(m_adminUsername)).WillByDefault(Invoke(
				[this](const std::string&) -> std::vector<std::string>
				{
					return {"Admin"};
				}
			));

			m_basicUsername = "Robin";
			ON_CALL(m_userRoleService, getUserRoles(m_basicUsername)).WillByDefault(Invoke(
				[this](const std::string&) -> std::vector<std::string>
				{
					return {"Basic"};
				}
			));
		}

		std::unique_ptr<IRoute> buildRoute(const std::string& method,
										   const std::string& uri,
										   UserRole role)
		{
			std::vector< std::function<std::unique_ptr<IRouteAccessValidator>()> > accessValidators;
			if (static_cast<unsigned int>(role) > 0)
			{
				accessValidators.push_back(m_tokenExpirationAccessValidator);
			}

			if (role == UserRole::ADMIN)
			{
				accessValidators.push_back(m_adminUserAccessValidator);
			}
			else if (role == UserRole::BASIC)
			{
				accessValidators.push_back(m_basicUserAccessValidator);
			}

			return m_routesFactory->buildRoute(method, uri, accessValidators, buildEndpointFactoryMethod(method, uri));
		}

		std::function< std::unique_ptr<IEndpoint>() > buildEndpointFactoryMethod(const std::string& method, const std::string& uri)
		{
			return [method, uri]() -> std::unique_ptr<IEndpoint>
				{
					auto endpoint = std::make_unique<MockEndpoint>();
					ON_CALL(*endpoint, executeProxy(_)).WillByDefault(Invoke(
						[method, uri](const EndpointRequestData& endpointRequestData) -> web_server::Reply*
						{
							std::stringstream ss;
							ss << "{";
							ss << "    \"method\": \"" << method << "\", ";
							ss << "    \"uri\": \"" << uri << "\" ";
							ss << "}";
							std::string content = ss.str();

							auto reply = std::make_unique<web_server::Reply>();
							reply->setStatus(web_server::Reply::OK);
							reply->setContent(content);
							return reply.release();
						}
					));

					return endpoint;
				};
		}

		web_server::Request buildRequest(const std::string& method,
										 const std::string& uri,
										 const std::string& username = "")
		{
			web_server::Request request;
			request.setHttpVersionMajor(1);
			request.setHttpVersionMinor(1);
			request.setMethod(method);
			request.setURI(uri);
			request.setContent("{}");

			if (!username.empty())
			{
				std::vector< std::pair<std::string, std::string> > authorizationClaims;
				authorizationClaims.push_back( std::make_pair(claim::SUBJECT, username) );
				authorizationClaims.push_back( std::make_pair(claim::ISSUED_AT, std::to_string(m_epochTimeService.getCurrentEpochTime())) );
				std::string token = m_tokenBuilderService->buildJWT(m_jwtKey, authorizationClaims);
				request.getHeaders().addHeader(web_server::AUTHORIZATION, "Bearer " + token);
			}

			return request;
		}

		std::string buildExpectedContentJSON(const std::string& method, const std::string& uri)
		{
			std::stringstream ss;
			ss << "{";
			ss << "    \"method\": \"" << method << "\", ";
			ss << "    \"uri\": \"" << uri << "\" ";
			ss << "}";

			return ss.str();
		}

	protected:
		std::unique_ptr<Router> m_router;

		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
		std::unique_ptr<jwt::TokenBuilderService> m_tokenBuilderService;
		std::string m_jwtKey;

		std::unique_ptr<RoutesFactory> m_routesFactory;
		StubEpochTimeService m_epochTimeService;
		MockUserRoleService m_userRoleService;
		std::string m_adminUsername;
		std::string m_basicUsername;

		std::function< std::unique_ptr<IRouteAccessValidator>() > m_tokenExpirationAccessValidator;
		std::function< std::unique_ptr<IRouteAccessValidator>() > m_adminUserAccessValidator;
		std::function< std::unique_ptr<IRouteAccessValidator>() > m_basicUserAccessValidator;
	};


	// Happy path requests
	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsersWithBasicRoleReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("GET", "/rest/api/users", m_basicUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("GET", "/rest/api/users"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsers123WithBasicRoleReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("GET", "/rest/api/users/123", m_basicUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("GET", "/rest/api/users/+id"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessPOSTForRESTAPIUsersWithAdminRoleReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("POST", "/rest/api/users/", m_adminUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("POST", "/rest/api/users"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessPUTForRESTAPIUsers456WithAdminRoleReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("PUT", "/rest/api/users/456", m_adminUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("PUT", "/rest/api/users/+id"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessDELETEForRESTAPIUsers789WithAdminRoleReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("DELETE", "/rest/api/users/789", m_adminUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("DELETE", "/rest/api/users/+id"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessPOSTForRESTAPIUsersLoginWithAnonymousRequestReturnsReplyStatusOK)
	{
		auto reply = m_router->process(buildRequest("POST", "/rest/api/users/login"));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::OK);
		EXPECT_TRUE(compareJSONs(buildExpectedContentJSON("POST", "/rest/api/users/login"), reply->getContent(), m_jsonAdapter));
	}


	// Requests without access due to user role
	TEST_F(RouterComponentTest, testProcessDELETEForRESTAPIUsers789WithAnonymousRequestReturnsReplyStatusForbidden)
	{
		auto reply = m_router->process(buildRequest("DELETE", "/rest/api/users/789"));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::FORBIDDEN);
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessDELETEForRESTAPIUsers789WithBasicRoleReturnsReplyStatusForbidden)
	{
		auto reply = m_router->process(buildRequest("DELETE", "/rest/api/users/789", m_basicUsername));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::FORBIDDEN);
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsers789WithAnonymousRequestReturnsReplyStatusForbidden)
	{
		auto reply = m_router->process(buildRequest("GET", "/rest/api/users/789"));
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::FORBIDDEN);
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}


	// Requests without access due to expired token
	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsersWithBasicRoleButExpiredTokenReturnsReplyStatusForbidden)
	{
		auto request = buildRequest("GET", "/rest/api/users", m_basicUsername);
		m_epochTimeService.addSeconds(601);
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply != NULL);
		EXPECT_EQ(reply->getStatus(), web_server::Reply::FORBIDDEN);
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}


	// Requests for not existing routes
	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsersIdReturnsNullReply)
	{
		ASSERT_TRUE(m_router->process(buildRequest("GET", "/rest/api/users/+id")) == NULL);
	}

	TEST_F(RouterComponentTest, testProcessGETForRESTAPIUsersSomethingReturnsNullReply)
	{
		ASSERT_TRUE(m_router->process(buildRequest("GET", "/rest/api/users/something")) == NULL);
	}

	TEST_F(RouterComponentTest, testProcessPUTForRESTAPIUsersLoginReturnsNullReply)
	{
		ASSERT_TRUE(m_router->process(buildRequest("PUT", "/rest/api/users/login")) == NULL);
	}


}}}
