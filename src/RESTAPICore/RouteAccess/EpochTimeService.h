#pragma once

#include "IEpochTimeService.h"


namespace systelab { namespace rest_api_core {

	class EpochTimeService : public IEpochTimeService
	{
	public:
		EpochTimeService();
		virtual ~EpochTimeService();

		long long getCurrentEpochTime() const override;
	};

}}
