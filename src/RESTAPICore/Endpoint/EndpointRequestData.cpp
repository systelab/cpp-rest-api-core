#include "stdafx.h"
#include "EndpointRequestData.h"


namespace systelab { namespace rest_api_core {

	EndpointRequestData::EndpointRequestData()
		:m_parameters()
		,m_content("")
		,m_headers()
		,m_queryStrings()
		,m_authorizationClaims()
	{
	}

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

	EndpointRequestData::EndpointRequestData(const EndpointRequestData& other)
		:m_parameters(other.m_parameters)
		,m_content(other.m_content)
		,m_headers(other.m_headers)
		,m_queryStrings(other.m_queryStrings)
		,m_authorizationClaims(other.m_authorizationClaims)
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

	void EndpointRequestData::setParameters(const EndpointRequestParams& parameters)
	{
		m_parameters = parameters;
	}

	void EndpointRequestData::setContent(const std::string& content)
	{
		m_content = content;
	}

	void EndpointRequestData::setHeaders(const systelab::web_server::RequestHeaders& headers)
	{
		m_headers = headers;
	}

	void EndpointRequestData::setQueryStrings(const systelab::web_server::RequestQueryStrings& queryStrings)
	{
		m_queryStrings = queryStrings;
	}

	void EndpointRequestData::setAuthorizationClaims(const EndpointRequestAuthorizationClaims& authorizationClaims)
	{
		m_authorizationClaims = authorizationClaims;
	}

}}

