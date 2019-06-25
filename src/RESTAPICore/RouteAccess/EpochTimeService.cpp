#include "stdafx.h"
#include "EpochTimeService.h"

#include <ctime>


namespace systelab { namespace rest_api_core {

	EpochTimeService::EpochTimeService()
	{
	}

	EpochTimeService::~EpochTimeService() = default;

	long long EpochTimeService::getCurrentEpochTime() const
	{
		return std::time(nullptr);
	}

}}
