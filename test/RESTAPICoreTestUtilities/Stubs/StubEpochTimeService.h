#pragma once

#include "../Mocks/RouteAccess/MockEpochTimeService.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class StubEpochTimeService : public MockEpochTimeService
	{
	public:
		StubEpochTimeService();
		virtual ~StubEpochTimeService();

		void addSeconds(unsigned int nSeconds);

	private:
		std::time_t m_currentTime;
	};

}}}

