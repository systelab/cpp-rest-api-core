#include "stdafx.h"
#include "TokenExpirationAccessValidator.h"

#include "Endpoint/ClaimConstants.h"
#include "Endpoint/EndpointRequestData.h"
#include "RouteAccess/EpochTimeService.h"

#include "TimeAdapter/ITimeAdapter.h"


namespace systelab { namespace rest_api_core {

	TokenExpirationAccessValidator::TokenExpirationAccessValidator(const systelab::time::ITimeAdapter& timeAdapter,
																   long expirationSeconds)
		:m_epochTimeService(std::make_unique<EpochTimeService>(timeAdapter))
		,m_expirationSeconds(expirationSeconds)
	{
	}

	TokenExpirationAccessValidator::~TokenExpirationAccessValidator() = default;

	bool TokenExpirationAccessValidator::hasAccess(EndpointRequestData& endpointRequestData) const
	{
		try
		{
			std::string iat = endpointRequestData.getAuthorizationClaims().getClaim(claim::ISSUED_AT);
			long long tokenEpoch = std::atoll(iat.c_str());
			long long expirationEpoch = tokenEpoch + m_expirationSeconds;
			long long currentEpoch = m_epochTimeService->getCurrentEpochTime();

			return (currentEpoch <= expirationEpoch);
		}
		catch (std::exception&)
		{
			return false;
		}
	}

}}
