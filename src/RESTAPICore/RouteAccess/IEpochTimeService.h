#pragma once


namespace systelab { namespace rest_api_core {
	
	class IEpochTimeService
	{
	public:
		virtual ~IEpochTimeService() = default;

		virtual long long getCurrentEpochTime() const = 0;
	};

}}
