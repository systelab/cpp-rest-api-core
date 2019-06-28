#pragma once

#include "RESTAPICore/Endpoint/EndpointRequestAuthorizationClaims.h"
#include "RESTAPICore/Endpoint/EndpointRequestParams.h"

#include "WebServerAdapterInterface/Model/RequestHeaders.h"
#include "WebServerAdapterInterface/Model/RequestQueryStrings.h"


namespace systelab { namespace rest_api_core {

	class EndpointRequestData
	{
	public:
		EndpointRequestData(const EndpointRequestParams&,
							const std::string&,
							const systelab::web_server::RequestHeaders&,
							const systelab::web_server::RequestQueryStrings&,
							const EndpointRequestAuthorizationClaims&);
		EndpointRequestData(const EndpointRequestData&);
		virtual ~EndpointRequestData();

		const EndpointRequestParams& getParameters() const;
		const std::string& getContent() const;
		const systelab::web_server::RequestHeaders& getHeaders() const;
		const systelab::web_server::RequestQueryStrings& getQueryStrings() const;
		const EndpointRequestAuthorizationClaims& getAuthorizationClaims() const;

	private:
		const EndpointRequestParams m_parameters;
		const std::string m_content;
		const systelab::web_server::RequestHeaders m_headers;
		const systelab::web_server::RequestQueryStrings m_queryStrings;
		const EndpointRequestAuthorizationClaims& m_authorizationClaims;
	};

}}

