#pragma once

#include "IAuthorizationDataBuilder.h"

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Router/RouteFragment.h"


namespace systelab { namespace jwt {
	class ITokenParserService;
}}

namespace systelab { namespace rest_api_core {

	class AuthorizationDataBuilder : public IAuthorizationDataBuilder
	{
	public:
		AuthorizationDataBuilder(const std::string& jwtKey,
								 const jwt::ITokenParserService&);
		virtual ~AuthorizationDataBuilder();

		std::unique_ptr<EndpointRequestAuthorizationClaims>
		buildAuthorizationData(const systelab::web_server::RequestHeaders&) const override;

	private:
		std::string m_jwtKey;
		const jwt::ITokenParserService& m_jwtParserService;
	};

}}
