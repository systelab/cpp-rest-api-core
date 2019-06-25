#include "stdafx.h"
#include "EndpointRequestData.h"


namespace systelab { namespace rest_api_core {

	EndpointRequestData::EndpointRequestData(const EndpointRequestParams& parameters,
											 const std::string& content,
											 const systelab::web_server::RequestHeaders& headers,
											 const systelab::web_server::RequestQueryStrings& queryStrings,
											 const EndpointRequestAuthorizationClaims& authorizationClaims)
		:m_parameters(parameters)
		,m_content(content)
		,m_headers(headers)
		,m_queryStrings(queryStrings)
		,m_authorizationClaims(authorizationClaims)
	{
	}
	
	EndpointRequestData::~EndpointRequestData() = default;

	const EndpointRequestParams& EndpointRequestData::getParameters() const
	{
		return m_parameters;
	}

	const std::string& EndpointRequestData::getContent() const
	{
		return m_content;
	}

	const systelab::web_server::RequestHeaders& EndpointRequestData::getHeaders() const
	{
		return m_headers;
	}

	const systelab::web_server::RequestQueryStrings& EndpointRequestData::getQueryStrings() const
	{
		return m_queryStrings;
	}

	const EndpointRequestAuthorizationClaims& EndpointRequestData::getAuthorizationClaims() const
	{
		return m_authorizationClaims;
	}

}}

