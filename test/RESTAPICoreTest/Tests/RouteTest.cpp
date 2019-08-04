#include "stdafx.h"
#include "RESTAPICore/Router/RoutesFactory.h"
#include "RESTAPICore/Router/IRoute.h"

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"


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
				[this]()-> std::unique_ptr<IEndpoint>
				{
					auto endpoint = std::make_unique<test_utility::MockEndpoint>();
					//ON_CALL(*endpoint, executeProxy(_)).WillByDefault(Return(m_request));
					return endpoint;
				};

			//m_route = m_routesFactory->buildRoute("POST", "/rest/api/test", {},
												  
		}

	protected:
		std::string m_jwtKey;
		std::unique_ptr<RoutesFactory> m_routesFactory;

		
		std::function< std::unique_ptr<IEndpoint>() > m_endpointFactoryMethod;

		std::unique_ptr<IRoute> m_route;

	};


	TEST_F(RouteTest, testEncodeStringReturnsExpectedValue)
	{
		//m_route->execute(m_request);
	}

}}}
