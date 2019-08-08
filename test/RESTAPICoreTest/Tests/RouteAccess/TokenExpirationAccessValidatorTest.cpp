#include "stdafx.h"
#include "RESTAPICore/RouteAccess/TokenExpirationAccessValidator.h"

#include "RESTAPICore/Endpoint/ClaimConstants.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"

#include "RESTAPICoreTestUtilities/Stubs/StubEpochTimeService.h"

#include <chrono>


using namespace testing;
using namespace systelab::rest_api_core::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	namespace {
		struct TokenExpirationAccessValidatorTestData
		{
			unsigned int expirationSeconds;
			unsigned int elapsedSeconds;
			bool expectedAccess;
		};

		std::vector< TokenExpirationAccessValidatorTestData> testData =
		{
			// Expiration 30 minutes => 1800 seconds
			{1800, 0, true},
			{1800, 1, true},
			{1800, 10, true},
			{1800, 100, true},
			{1800, 1000, true},
			{1800, 1799, true},
			{1800, 1800, true},
			{1800, 1801, false},
			{1800, 10000, false},
			{1800, 100000, false},

			// Expiration 2 hours => 7200 seconds
			{7200, 0, true},
			{7200, 1, true},
			{7200, 10, true},
			{7200, 100, true},
			{7200, 1000, true},
			{7200, 5000, true},
			{7200, 7199, true},
			{7200, 7200, true},
			{7200, 7201, false},
			{7200, 10000, false},
			{7200, 100000, false},
		};
	}

	class TokenExpirationAccessValidatorTest : public TestWithParam<TokenExpirationAccessValidatorTestData>
	{
	public:
		void SetUp()
		{
			unsigned int expirationSeconds = 1800;
			m_validator = std::make_unique<TokenExpirationAccessValidator>(expirationSeconds, m_epochTimeService);
		}

		EndpointRequestData buildEndpointRequestDataWithIATOfCurrentTime()
		{
			std::map<std::string, std::string> claims;
			claims.insert( std::make_pair(claim::ISSUED_AT, std::to_string(m_epochTimeService.getCurrentEpochTime())) );

			return buildEndpointRequestData(claims);
		}

		EndpointRequestData buildEndpointRequestData(std::map<std::string, std::string> claims)
		{
			EndpointRequestAuthorizationClaims authorizationClaims;
			for (auto claim : claims)
			{
				authorizationClaims.addClaim(claim.first, claim.second);
			}

			EndpointRequestData endpointRequestData;
			endpointRequestData.setAuthorizationClaims(authorizationClaims);

			return endpointRequestData;
		}

	protected:
		std::unique_ptr<TokenExpirationAccessValidator> m_validator;
		StubEpochTimeService m_epochTimeService;
		long m_expirationSeconds;
	};


	TEST_P(TokenExpirationAccessValidatorTest, testHasAccessReturnsExpectedValue)
	{
		m_validator = std::make_unique<TokenExpirationAccessValidator>(GetParam().expirationSeconds, m_epochTimeService);
		EndpointRequestData endpointRequestData = buildEndpointRequestDataWithIATOfCurrentTime();

		m_epochTimeService.addSeconds(GetParam().elapsedSeconds);

		ASSERT_EQ(GetParam().expectedAccess, m_validator->hasAccess(endpointRequestData));
	}

	TEST_F(TokenExpirationAccessValidatorTest, testHasAccessReturnsFalseWhenIATClaimNotFound)
	{
		EndpointRequestData endpointRequestData = buildEndpointRequestData({});
		ASSERT_FALSE(m_validator->hasAccess(endpointRequestData));
	}

	TEST_F(TokenExpirationAccessValidatorTest, testHasAccessReturnsFalseWhenIATClaimIsInvalid)
	{
		EndpointRequestData endpointRequestData = buildEndpointRequestData({ {claim::ISSUED_AT, "InvalidValue"} });
		ASSERT_FALSE(m_validator->hasAccess(endpointRequestData));
	}

	INSTANTIATE_TEST_CASE_P(RESTAPICore, TokenExpirationAccessValidatorTest, testing::ValuesIn(testData));

}}}
