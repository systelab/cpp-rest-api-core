#include "stdafx.h"
#include "TokenExpirationAccessValidator.h"

#include "Endpoint/ClaimConstants.h"
#include "Endpoint/EndpointRequestData.h"
#include "RouteAccess/IEpochTimeService.h"


namespace systelab { namespace rest_api_core {

	TokenExpirationAccessValidator::TokenExpirationAccessValidator(long expirationSeconds,
																   IEpochTimeService& epochTimeService)
		:m_expirationSeconds(expirationSeconds)
		,m_epochTimeService(epochTimeService)
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
			long long currentEpoch = m_epochTimeService.getCurrentEpochTime();

			return (currentEpoch <= expirationEpoch);
		}
		catch (std::exception&)
		{
			return false;
		}
	}

}}
