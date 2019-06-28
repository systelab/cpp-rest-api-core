#include "stdafx.h"
#include "RESTAPICore/Endpoint/EndpointRequestAuthorizationClaims.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::rest_api_core;

namespace systelab { namespace test_utility {

	template<>
	AssertionResult EntityComparator::operator() (const EndpointRequestAuthorizationClaims& expected, const EndpointRequestAuthorizationClaims& actual) const
	{
		COMPARATOR_ASSERT_EQUAL(expected, actual, getClaimCount());
		auto claimNames = expected.getClaimNames();
		for (auto name : claimNames)
		{
			if (!actual.hasClaim(name))
			{
				return AssertionFailure() << "Expected claim '" << name << "' not found.";
			}

			std::string expectedValue = expected.getClaim(name);
			std::string actualValue = actual.getClaim(name);
			if (expectedValue != actualValue)
			{
				return AssertionFailure() << "Different value for claim '" << name << "': "
										  << "expected='" << expectedValue << "', "
										  << "actual='" << actualValue << "'";
			}
		}

		return AssertionSuccess();
	}

}}
