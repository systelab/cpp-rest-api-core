#include "stdafx.h"
#include "RESTAPICore/RouteAccess/EpochTimeService.h"

#include "TimeAdapterTestUtilities/Stubs/StubTimeAdapter.h"


using namespace testing;
using namespace systelab::time::test_utility;

namespace systelab { namespace rest_api_core { namespace unit_test {

	class EpochTimeServiceTest : public Test
	{
		void SetUp()
		{
			setUpTimeAdapter();
			m_service = std::make_unique<EpochTimeService>(m_timeAdapter);
		}

		void setUpTimeAdapter()
		{
			m_currentEpoch = 1586272783;
			auto currentTimePoint = std::chrono::system_clock::time_point{ std::chrono::seconds{m_currentEpoch} };
			m_timeAdapter.setCurrentTime(currentTimePoint);
		}

	protected:
		std::unique_ptr<IEpochTimeService> m_service;

		StubTimeAdapter m_timeAdapter;
		long long m_currentEpoch;
	};


	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsValueConfiguredOnTimeAdapter)
	{
		ASSERT_EQ(m_currentEpoch, m_service->getCurrentEpochTime());
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsSameValueOnConsecutiveCalls)
	{
		time_t currentTime1 = m_service->getCurrentEpochTime();
		time_t currentTime2 = m_service->getCurrentEpochTime();
		ASSERT_EQ(currentTime2, currentTime1);
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsValueConfiguredOnTimeAdapterAfter1Millisecond)
	{
		m_timeAdapter.addMilliseconds(1);
		ASSERT_EQ(m_currentEpoch, m_service->getCurrentEpochTime());
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsValueConfiguredOnTimeAdapterAfter999Milliseconds)
	{
		m_timeAdapter.addMilliseconds(999);
		ASSERT_EQ(m_currentEpoch, m_service->getCurrentEpochTime());
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsNextEpochAfter1Second)
	{
		m_timeAdapter.addSeconds(1);
		ASSERT_EQ(m_currentEpoch + 1, m_service->getCurrentEpochTime());
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentEpochTimeReturnsNextEpochAfter1SecondAndAHalf)
	{
		m_timeAdapter.addMilliseconds(1500);
		ASSERT_EQ(m_currentEpoch + 1, m_service->getCurrentEpochTime());
	}

	TEST_F(EpochTimeServiceTest, testGetCurrentTimeEpochReturnsTime25SecondsGreaterAfter25Seconds)
	{
		time_t currentTime1 = m_service->getCurrentEpochTime();
		m_timeAdapter.addSeconds(25);
		time_t currentTime2 = m_service->getCurrentEpochTime();
		ASSERT_EQ(difftime(currentTime2, currentTime1), 25);
	}

}}}
