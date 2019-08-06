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

	unsigned int EndpointRequestAuthorizationClaims::getClaimCount() const
	{
		return (unsigned int) m_claims.size();
	}

	std::vector<std::string> EndpointRequestAuthorizationClaims::getClaimNames() const
	{
		std::vector<std::string> names;
		for (auto claim : m_claims)
		{
			names.push_back(claim.first);
		}

		return names;
	}

	bool EndpointRequestAuthorizationClaims::hasClaim(const std::string& name) const
	{
		return (m_claims.find(name) != m_claims.end());
	}

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

	void EndpointRequestAuthorizationClaims::addClaim(const std::string& name, const std::string& value)
	{
		m_claims.insert(std::make_pair(name, value));
	}

	EndpointRequestAuthorizationClaims& EndpointRequestAuthorizationClaims::operator=(const EndpointRequestAuthorizationClaims& other)
	{
		m_claims = other.m_claims;
		return *this;
	}

	bool operator== (const EndpointRequestAuthorizationClaims& lhs, const EndpointRequestAuthorizationClaims& rhs)
	{
		if (lhs.m_claims != rhs.m_claims)
		{
			return false;
		}

		return true;
	}

	bool operator!= (const EndpointRequestAuthorizationClaims& lhs, const EndpointRequestAuthorizationClaims& rhs)
	{
		return !(lhs == rhs);
	}

}}

