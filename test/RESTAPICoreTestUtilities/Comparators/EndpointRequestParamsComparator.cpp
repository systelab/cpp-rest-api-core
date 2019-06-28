#include "stdafx.h"
#include "RESTAPICore/Endpoint/EndpointRequestParams.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::rest_api_core;

namespace systelab { namespace test_utility {

	template<>
	AssertionResult EntityComparator::operator() (const EndpointRequestParams& expected, const EndpointRequestParams& toTest) const
	{
		// String parameters
		COMPARATOR_ASSERT_EQUAL(expected, toTest, getStringParameterCount());
		auto stringParameterNames = expected.getStringParameterNames();
		for (auto parameterName : stringParameterNames)
		{
			if (!toTest.hasStringParameter(parameterName))
			{
				return AssertionFailure() << "Expected string parameter '" << parameterName << "' not found.";
			}

			std::string expectedValue = expected.getStringParameter(parameterName);
			std::string toTestValue = toTest.getStringParameter(parameterName);
			if (expectedValue != toTestValue)
			{
				return AssertionFailure() << "Different value for string parameter '" << parameterName << "': "
										  << "expected='" << expectedValue << "', actual='" << toTestValue << "'";
			}
		}


		// Numeric parameters
		COMPARATOR_ASSERT_EQUAL(expected, toTest, getNumericParameterCount());
		auto numericParameterNames = expected.getNumericParameterNames();
		for (auto parameterName : numericParameterNames)
		{
			if (!toTest.hasNumericParameter(parameterName))
			{
				return AssertionFailure() << "Expected numeric parameter '" << parameterName << "' not found.";
			}

			unsigned int expectedValue = expected.getNumericParameter(parameterName);
			unsigned int toTestValue = toTest.getNumericParameter(parameterName);
			if (expectedValue != toTestValue)
			{
				return AssertionFailure() << "Different value for numeric parameter '" << parameterName << "': "
										  << "expected=" << expectedValue << ", actual=" << toTestValue;
			}
		}

		return AssertionSuccess();
	}

}}
