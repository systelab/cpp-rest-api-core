#pragma once


namespace systelab { namespace rest_api_core {

	class EndpointRequestAuthorizationClaims
	{
	public:
		EndpointRequestAuthorizationClaims();
		EndpointRequestAuthorizationClaims(const std::vector< std::pair<std::string, std::string> >&);
		EndpointRequestAuthorizationClaims(const EndpointRequestAuthorizationClaims&);
		virtual ~EndpointRequestAuthorizationClaims();

		unsigned int getClaimCount() const;
		std::vector<std::string> getClaimNames() const;
		bool hasClaim(const std::string& name) const;
		std::string getClaim(const std::string& name) const;

	private:
		std::map<std::string, std::string> m_claims;
	};

}}

