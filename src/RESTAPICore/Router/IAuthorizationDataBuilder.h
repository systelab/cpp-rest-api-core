#pragma once

namespace systelab { namespace web_server {
	class RequestHeaders;
}}

namespace systelab { namespace rest_api_core {

	class EndpointRequestAuthorizationClaims;

	class IAuthorizationDataBuilder
	{
	public:
		virtual ~IAuthorizationDataBuilder() = default;

		virtual std::unique_ptr<EndpointRequestAuthorizationClaims>
			buildAuthorizationData(const systelab::web_server::RequestHeaders&) const = 0;
	};

}}
