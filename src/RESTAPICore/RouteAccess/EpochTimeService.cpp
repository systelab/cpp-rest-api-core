#include "stdafx.h"
#include "EpochTimeService.h"

#include "TimeAdapter/ITimeAdapter.h"
#include <ctime>


namespace systelab { namespace rest_api_core {

	EpochTimeService::EpochTimeService(const systelab::time::ITimeAdapter& timeAdapter)
		:m_timeAdapter(timeAdapter)
	{
	}

	EpochTimeService::~EpochTimeService() = default;

	long long EpochTimeService::getCurrentEpochTime() const
	{
		return std::chrono::system_clock::to_time_t(m_timeAdapter.getCurrentUTCTime());
	}

}}
