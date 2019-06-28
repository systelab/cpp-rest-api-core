#pragma once


namespace systelab { namespace rest_api_core {

	class EndpointRequestAuthorizationClaims
	{
	public:
		EndpointRequestAuthorizationClaims();
		EndpointRequestAuthorizationClaims(const EndpointRequestAuthorizationClaims&);
		EndpointRequestAuthorizationClaims(const std::vector< std::pair<std::string, std::string> >&);
		virtual ~EndpointRequestAuthorizationClaims();

		unsigned int getClaimCount() const;
		std::vector<std::string> getClaimNames() const;
		bool hasClaim(const std::string& name) const;
		std::string getClaim(const std::string& name) const;

		EndpointRequestAuthorizationClaims& operator= (const EndpointRequestAuthorizationClaims&);
		friend bool operator== (const EndpointRequestAuthorizationClaims&, const EndpointRequestAuthorizationClaims&);
		friend bool operator!= (const EndpointRequestAuthorizationClaims&, const EndpointRequestAuthorizationClaims&);

	private:
		std::map<std::string, std::string> m_claims;
	};

}}

