#include "stdafx.h"
#include "RESTAPICore/Router/Router.h"

#include "RESTAPICoreTestUtilities/Mocks/Router/MockRoute.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::rest_api_core::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	class RouterTest : public Test
	{
	public:
		void SetUp()
		{
			m_router = std::make_unique<Router>();
			m_router->addRoute(buildRoute("GET"));
			m_router->addRoute(buildRoute("POST"));
			m_router->addRoute(buildRoute("PUT"));
			m_router->addRoute(buildRoute("DELETE"));
		}
	
		std::unique_ptr<IRoute> buildRoute(const std::string& method)
		{
			auto route = std::make_unique<MockRoute>();
			ON_CALL(*route, executeProxy(_)).WillByDefault(Invoke(
				[method](const web_server::Request& request) -> web_server::Reply*
				{
					if (request.getMethod() == method)
					{
						return new web_server::Reply(web_server::Reply::OK, {}, method);
					}

					return NULL;
				}
			));

			return route;
		}

	protected:
		std::unique_ptr<Router> m_router;
	};


	TEST_F(RouterTest, testProcessGETRequestReturnsReplyStatusOKAndGETOnContent)
	{
		web_server::Request request;
		request.setMethod("GET");
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(reply->getStatus(), web_server::Reply::OK);
		ASSERT_EQ("GET", reply->getContent());
	}

	TEST_F(RouterTest, testProcessPOSTRequestReturnsReplyStatusOKAndPOSTOnContent)
	{
		web_server::Request request;
		request.setMethod("POST");
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(reply->getStatus(), web_server::Reply::OK);
		ASSERT_EQ("POST", reply->getContent());
	}

	TEST_F(RouterTest, testProcessPUTRequestReturnsReplyStatusOKAndPUTOnContent)
	{
		web_server::Request request;
		request.setMethod("PUT");
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(reply->getStatus(), web_server::Reply::OK);
		ASSERT_EQ("PUT", reply->getContent());
	}

	TEST_F(RouterTest, testProcessDELETERequestReturnsReplyStatusOKAndDELETEOnContent)
	{
		web_server::Request request;
		request.setMethod("DELETE");
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_EQ(reply->getStatus(), web_server::Reply::OK);
		ASSERT_EQ("DELETE", reply->getContent());
	}

	TEST_F(RouterTest, testProcessPATCHRequestReturnsNull)
	{
		web_server::Request request;
		request.setMethod("PATCH");
		auto reply = m_router->process(request);
		ASSERT_TRUE(reply == NULL);
	}

}}}
