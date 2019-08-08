#include "stdafx.h"
#include "StubEpochTimeService.h"

#include <chrono>


using namespace testing;

namespace systelab { namespace rest_api_core { namespace test_utility {

	StubEpochTimeService::StubEpochTimeService()
		:m_currentTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
	{
		ON_CALL(*this, getCurrentEpochTime()).WillByDefault(Invoke(
			[this]() -> std::time_t
			{
				return m_currentTime;
			}
		));
	}

	StubEpochTimeService::~StubEpochTimeService() = default;

	void StubEpochTimeService::addSeconds(unsigned int nSeconds)
	{
		auto currentTimePoint = std::chrono::system_clock::from_time_t(m_currentTime);
		m_currentTime = std::chrono::system_clock::to_time_t(currentTimePoint + std::chrono::seconds(nSeconds));
	}

}}}

