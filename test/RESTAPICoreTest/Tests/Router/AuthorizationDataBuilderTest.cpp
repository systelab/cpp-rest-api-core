#include "stdafx.h"
#include "RESTAPICore/Router/AuthorizationDataBuilder.h"

#include "JWTUtils/Services/TokenParserService.h"
#include "RapidJSONAdapter/JSONAdapter.h"
#include "RESTAPICore/Endpoint/EndpointRequestAuthorizationClaims.h"

#include "WebServerAdapterInterface/Model/Headers.h"
#include "WebServerAdapterInterface/Model/RequestHeaders.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	namespace {

		struct AuthorizationDataBuilderTestData
		{
			std::map<std::string, std::string> headers;
			std::map<std::string, std::string> expectedClaims;
		};

		std::vector< AuthorizationDataBuilderTestData> testData =
		{
			{
				{ {web_server::CONTENT_TYPE, "application/json"} },
				{ }
			},
			{
				{ {web_server::AUTHORIZATION, "InvalidValue"} },
				{ }
			},
			{
				{ {web_server::AUTHORIZATION, "Basic anNtaXRoOm15cGFzc3dvcmQ="} },
				{ }
			},
			{
				{ {web_server::AUTHORIZATION, "Bearer anNtaXRoOm15cGFzc3dvcmQ="} },
				{ }
			},
			{
				{ {web_server::AUTHORIZATION, "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
											  "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoiMTUxNjIzOTAyMiJ9."
											  "SQ8-pQRMliyILSz_9n8f-Q2-viYWPMdPVmy5a6UlyWU"} },
				{ {"sub", "1234567890"}, {"name", "John Doe"}, {"iat", "1516239022"} }
			},
			{
				{ {web_server::CONTENT_TYPE, "application/json"},
				  {web_server::AUTHORIZATION, "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
											  "eyJuYW1lIjoiUGV0ZXIgUGFya2VyIn0."
											  "7grTTYGH6KLzV9uOOEawERIFTKlF3NgdmA3sptiSjD0"} },
				{ {"name", "Peter Parker"} }
			}
		};
	}

	class AuthorizationDataBuilderTest : public TestWithParam<AuthorizationDataBuilderTestData>
	{
	public:
		void SetUp()
		{
			m_jwtKey = "my-super-secret-key";
			m_tokenParserService = std::make_unique<jwt::TokenParserService>(m_jsonAdapter);
			m_builder = std::make_unique<AuthorizationDataBuilder>(m_jwtKey, *m_tokenParserService);
		}

	protected:
		std::unique_ptr<AuthorizationDataBuilder> m_builder;

		std::string m_jwtKey;
		std::unique_ptr<jwt::TokenParserService> m_tokenParserService;
		json::rapidjson::JSONAdapter m_jsonAdapter;
	};


	TEST_P(AuthorizationDataBuilderTest, testBuildAuthorizationData)
	{
		web_server::RequestHeaders requestHeaders;
		for (auto header : GetParam().headers)
		{
			requestHeaders.addHeader(header.first, header.second);
		}

		EndpointRequestAuthorizationClaims expectedClaims;
		for (auto claim : GetParam().expectedClaims)
		{
			expectedClaims.addClaim(claim.first, claim.second);
		}

		auto claims = m_builder->buildAuthorizationData(requestHeaders);
		ASSERT_TRUE(claims != NULL);
		ASSERT_TRUE(EntityComparator()(expectedClaims, *claims));
	}

	INSTANTIATE_TEST_CASE_P(RESTAPICore, AuthorizationDataBuilderTest, testing::ValuesIn(testData));

}}}
