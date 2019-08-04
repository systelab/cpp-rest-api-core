#pragma once

#include "RESTAPICore/Endpoint/EndpointRequestAuthorizationClaims.h"
#include "RESTAPICore/Endpoint/EndpointRequestParams.h"

#include "WebServerAdapterInterface/Model/RequestHeaders.h"
#include "WebServerAdapterInterface/Model/RequestQueryStrings.h"


namespace systelab { namespace rest_api_core {

	class EndpointRequestData
	{
	public:
		EndpointRequestData();
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

		void setParameters(const EndpointRequestParams&);
		void setContent(const std::string&);
		void setHeaders(const systelab::web_server::RequestHeaders&);
		void setQueryStrings(const systelab::web_server::RequestQueryStrings&);
		void setAuthorizationClaims(const EndpointRequestAuthorizationClaims&);

	private:
		EndpointRequestParams m_parameters;
		std::string m_content;
		systelab::web_server::RequestHeaders m_headers;
		systelab::web_server::RequestQueryStrings m_queryStrings;
		EndpointRequestAuthorizationClaims m_authorizationClaims;
	};

}}

