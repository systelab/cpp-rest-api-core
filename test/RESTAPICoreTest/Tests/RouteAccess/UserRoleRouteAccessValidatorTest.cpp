#include "stdafx.h"
#include "RESTAPICore/RouteAccess/UserRoleRouteAccessValidator.h"

#include "RESTAPICore/Endpoint/ClaimConstants.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"

#include "RESTAPICoreTestUtilities/Mocks/RouteAccess/MockUserRoleService.h"


using namespace testing;
using namespace systelab::rest_api_core::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	namespace {
		struct UserRoleRouteAccessValidatorTestData
		{
			std::vector<std::string> allowedRoles;
			std::vector<std::string> userRoles;
			bool expectedAccess;
		};

		std::vector<UserRoleRouteAccessValidatorTestData> testData =
		{
			// Super, manager and basic allowed
			{ {"SUPER", "MANAGER", "BASIC"}, {"SUPER"}, true},
			{ {"SUPER", "MANAGER", "BASIC"}, {"MANAGER"}, true},
			{ {"SUPER", "MANAGER", "BASIC"}, {"BASIC"}, true},
			{ {"SUPER", "MANAGER", "BASIC"}, {"BASIC", "EXTERNAL"}, true},
			{ {"SUPER", "MANAGER", "BASIC"}, {"EXTERNAL"}, false},
			{ {"SUPER", "MANAGER", "BASIC"}, {"ANOTHER", "EXTERNAL"}, false},
			{ {"SUPER", "MANAGER", "BASIC"}, {}, false},

			// Only super allowed
			{ {"SUPER"}, {"SUPER"}, true},
			{ {"SUPER"}, {"MANAGER"}, false},
			{ {"SUPER"}, {"BASIC"}, false},
			{ {"SUPER"}, {"SUPER", "MANAGER", "BASIC"}, true},
			{ {"SUPER"}, {"SUPER", "MANAGER"}, true},
			{ {"SUPER"}, {"MANAGER", "BASIC"}, false},
			{ {"SUPER"}, {}, false},

			// No one allowed
			{ {}, {"SUPER"}, false},
			{ {}, {"MANAGER"}, false},
			{ {}, {"BASIC"}, false},
			{ {}, {"BASIC", "EXTERNAL"}, false},
			{ {}, {"EXTERNAL"}, false},
			{ {}, {"ANOTHER", "EXTERNAL"}, false},
			{ {}, {}, false},
		};
	}

	class UserRoleRouteAccessValidatorTest : public TestWithParam<UserRoleRouteAccessValidatorTestData>
	{
	public:
		void SetUp()
		{
			m_username = "jsmith";
			ON_CALL(m_userRoleService, getUserRoles(_)).WillByDefault(Return(std::vector<std::string>()));
			ON_CALL(m_userRoleService, getUserRoles(m_username)).WillByDefault(Invoke(
				[this](const std::string&) -> std::vector<std::string>
				{
					return m_userRoles;
				}
			));

			m_validator = std::make_unique<UserRoleRouteAccessValidator>(std::vector<std::string>(), m_userRoleService);
		}

		EndpointRequestData buildEndpointRequestDataWithValidSUBJECT()
		{
			std::map<std::string, std::string> claims;
			claims.insert(std::make_pair(claim::SUBJECT, m_username));

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
		std::unique_ptr<UserRoleRouteAccessValidator> m_validator;
		MockUserRoleService m_userRoleService;

		std::string m_username;
		std::vector<std::string> m_userRoles;
	};


	TEST_P(UserRoleRouteAccessValidatorTest, testHasAccessReturnsExpectedValue)
	{
		m_userRoles = GetParam().userRoles;
		m_validator = std::make_unique<UserRoleRouteAccessValidator>(GetParam().allowedRoles, m_userRoleService);
		EndpointRequestData endpointRequestData = buildEndpointRequestDataWithValidSUBJECT();

		ASSERT_EQ(GetParam().expectedAccess, m_validator->hasAccess(endpointRequestData));
	}

	TEST_F(UserRoleRouteAccessValidatorTest, testHasAccessReturnsFalseWhenSUBJECTClaimNotFound)
	{
		EndpointRequestData endpointRequestData = buildEndpointRequestData({});
		ASSERT_FALSE(m_validator->hasAccess(endpointRequestData));
	}

	TEST_F(UserRoleRouteAccessValidatorTest, testHasAccessReturnsFalseWhenSUBJECTClaimIsInvalid)
	{
		EndpointRequestData endpointRequestData = buildEndpointRequestData({ {claim::SUBJECT, "InvalidUsername"} });
		ASSERT_FALSE(m_validator->hasAccess(endpointRequestData));
	}

	INSTANTIATE_TEST_CASE_P(RESTAPICore, UserRoleRouteAccessValidatorTest, testing::ValuesIn(testData));

}}}
