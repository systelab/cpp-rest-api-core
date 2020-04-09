#pragma once

#include "IRouteAccessValidator.h"


namespace systelab { namespace time {
	class ITimeAdapter;
}}

namespace systelab { namespace rest_api_core {

	class IEpochTimeService;

	class TokenExpirationAccessValidator : public IRouteAccessValidator
	{
	public:
		TokenExpirationAccessValidator(const systelab::time::ITimeAdapter&, long expirationSeconds);
		virtual ~TokenExpirationAccessValidator();

		bool hasAccess(EndpointRequestData&) const override;

	private:
		std::unique_ptr<IEpochTimeService> m_epochTimeService;
		long m_expirationSeconds;
	};

}}
