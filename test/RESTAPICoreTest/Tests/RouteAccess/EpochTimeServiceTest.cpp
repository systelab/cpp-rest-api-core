#include "stdafx.h"
#include "RESTAPICore/RouteAccess/EpochTimeService.h"

#include <chrono>
#include <thread>


using namespace testing;

namespace systelab { namespace rest_api_core { namespace unit_test {

	class EpochTimeServiceTest : public Test
	{
	protected:
		rest_api_core::EpochTimeService m_service;
	};


	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsGreaterOrEqualTimeOnSecondCall)
	{
		time_t currentTime1 = m_service.getCurrentEpochTime();
		time_t currentTime2 = m_service.getCurrentEpochTime();
		ASSERT_GE(currentTime2, currentTime1);
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentTimeEpochReturnsTime2SecondsGreaterAfterWaiting2Seconds)
	{
		time_t currentTime1 = m_service.getCurrentEpochTime();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		time_t currentTime2 = m_service.getCurrentEpochTime();
		ASSERT_GE(difftime(currentTime2, currentTime1), 2);
	}

}}}
