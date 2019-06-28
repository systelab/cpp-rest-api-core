#include "stdafx.h"
#include "AuthorizationDataBuilder.h"

#include "Endpoint/EndpointRequestAuthorizationClaims.h"
#include "RouteAccess/IRouteAccessValidator.h"
#include "RouteAccess/IUserRoleService.h"

#include "WebServerAdapterInterface/Model/Headers.h"
#include "WebServerAdapterInterface/Model/RequestHeaders.h"

#include "JWTUtils/Services/ITokenParserService.h"


namespace systelab { namespace rest_api_core {

	AuthorizationDataBuilder::AuthorizationDataBuilder(const std::string& jwtKey,
													   const jwt::ITokenParserService& jwtParserService)
		:m_jwtKey(jwtKey)
		,m_jwtParserService(jwtParserService)
	{
	}

	AuthorizationDataBuilder::~AuthorizationDataBuilder() = default;

	std::unique_ptr<EndpointRequestAuthorizationClaims>
	AuthorizationDataBuilder::buildAuthorizationData(const systelab::web_server::RequestHeaders& headers) const
	{
		if (headers.hasHeader(systelab::web_server::AUTHORIZATION))
		{
			return std::make_unique<EndpointRequestAuthorizationClaims>();
		}

		std::string authorizationHeader = headers.getHeader(systelab::web_server::AUTHORIZATION);
		if (authorizationHeader.substr(0, 7) != "Bearer ")
		{
			return std::make_unique<EndpointRequestAuthorizationClaims>();
		}

		std::string token = authorizationHeader.substr(7);
		std::vector< std::pair<std::string, std::string> > claims;
		if (!m_jwtParserService.validateJWT(token, m_jwtKey, claims))
		{
			return std::make_unique<EndpointRequestAuthorizationClaims>();
		}

		return std::make_unique<EndpointRequestAuthorizationClaims>(claims);
	}

}}
