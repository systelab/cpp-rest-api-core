#pragma once

#include "RESTAPICore/RouteAccess/IRouteAccessValidator.h"

#include "RESTAPICore/Endpoint/EndpointRequestData.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockRouteAccessValidator : public IRouteAccessValidator
	{
	public:
		MockRouteAccessValidator();
		virtual ~MockRouteAccessValidator();

		MOCK_CONST_METHOD1(hasAccess, bool(EndpointRequestData&));
	};

}}}

