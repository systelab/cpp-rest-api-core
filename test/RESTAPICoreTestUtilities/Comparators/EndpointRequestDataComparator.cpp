#include "stdafx.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::rest_api_core;

namespace systelab { namespace test_utility {

	template<>
	AssertionResult EntityComparator::operator() (const EndpointRequestData& expected, const EndpointRequestData& actual) const
	{
		auto& expectedParameters = expected.getParameters();
		auto& actualParameters = actual.getParameters();
		AssertionResult parametersResult = EntityComparator()(expectedParameters, actualParameters);
		if (!parametersResult)
		{
			return AssertionFailure() << "Different parameters data: " << parametersResult.message();
		}

		COMPARATOR_ASSERT_EQUAL(expected, actual, getContent());

		auto& expectedHeaders = expected.getHeaders();
		auto& actualHeaders = actual.getHeaders();
		AssertionResult headersResult = EntityComparator()(expectedHeaders, actualHeaders);
		if (!headersResult)
		{
			return AssertionFailure() << "Different headers data: " << headersResult.message();
		}

		auto& expectedQueryStrings = expected.getQueryStrings();
		auto& actualQueryStrings = actual.getQueryStrings();
		AssertionResult queryStringsResult = EntityComparator()(expectedQueryStrings, actualQueryStrings);
		if (!queryStringsResult)
		{
			return AssertionFailure() << "Different query strings data: " << queryStringsResult.message();
		}

		auto& expectedAuthorizationClaims = expected.getAuthorizationClaims();
		auto& actualAuthorizationClaims = actual.getAuthorizationClaims();
		AssertionResult authorizationClaimsResult = EntityComparator()(expectedAuthorizationClaims, actualAuthorizationClaims);
		if (!authorizationClaimsResult)
		{
			return AssertionFailure() << "Different authorization claims data: " << authorizationClaimsResult.message();
		}

		return AssertionSuccess();
	}

}}
