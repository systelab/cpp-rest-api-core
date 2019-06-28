#pragma once

#include "RESTAPICore/RouteAccess/IEpochTimeService.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockEpochTimeService : public IEpochTimeService
	{
	public:
		MockEpochTimeService();
		virtual ~MockEpochTimeService();

		MOCK_CONST_METHOD0(getCurrentEpochTime, long long ());
	};

}}}

