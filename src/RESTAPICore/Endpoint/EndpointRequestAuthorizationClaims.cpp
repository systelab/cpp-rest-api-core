#include "stdafx.h"
#include "EndpointRequestAuthorizationClaims.h"


namespace systelab { namespace rest_api_core {

	EndpointRequestAuthorizationClaims::EndpointRequestAuthorizationClaims()
		:m_claims()
	{
	}

	EndpointRequestAuthorizationClaims::EndpointRequestAuthorizationClaims(const std::vector< std::pair<std::string, std::string> >& claims)
		:m_claims()
	{
		for (auto claim : claims)
		{
			m_claims.insert(claim);
		}
	}

	EndpointRequestAuthorizationClaims::EndpointRequestAuthorizationClaims(const EndpointRequestAuthorizationClaims& other)
		:m_claims(other.m_claims)
	{
	}

	EndpointRequestAuthorizationClaims::~EndpointRequestAuthorizationClaims() = default;

	std::string EndpointRequestAuthorizationClaims::getClaim(const std::string& name) const
	{
		auto it = m_claims.find(name);
		if (it != m_claims.end())
		{
			return it->second;
		}
		else
		{
			throw std::runtime_error("Claim '" + name + "' not found");
		}
	}

}}

