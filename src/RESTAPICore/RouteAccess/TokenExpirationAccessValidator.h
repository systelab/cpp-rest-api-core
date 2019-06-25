#pragma once

#include "IRouteAccessValidator.h"


namespace systelab { namespace rest_api_core {

	class IEpochTimeService;

	class TokenExpirationAccessValidator : public IRouteAccessValidator
	{
	public:
		TokenExpirationAccessValidator(long expirationSeconds,
									   IEpochTimeService&);
		virtual ~TokenExpirationAccessValidator();

		bool hasAccess(EndpointRequestData&) const override;

	private:
		long m_expirationSeconds;
		IEpochTimeService& m_epochTimeService;
	};

}}
