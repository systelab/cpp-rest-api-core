#include "stdafx.h"
#include "RoutesFactory.h"

#include "Router/AuthorizationDataBuilder.h"
#include "Router/Route.h"

#include "JWTUtils/Services/TokenParserService.h"
#include "RapidJSONAdapter/JSONAdapter.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace rest_api_core {

	RoutesFactory::RoutesFactory(const std::string& jwtKey)
		:m_jsonAdapter(std::make_unique<json::rapidjson::JSONAdapter>())
		,m_jwtParserService(std::make_unique<jwt::TokenParserService>(*m_jsonAdapter))
		,m_authorizationDataBuilder(std::make_unique<AuthorizationDataBuilder>(jwtKey, *m_jwtParserService))
	{
		std::map<std::string, std::string> headers = { {std::string("Content-Type"), std::string("application/json")} };
		m_unauthorizedReply = std::make_unique<systelab::web_server::Reply>(systelab::web_server::Reply::UNAUTHORIZED, headers, "{}");
		m_forbiddenReply = std::make_unique<systelab::web_server::Reply>(systelab::web_server::Reply::FORBIDDEN, headers, "{}");
	}

	RoutesFactory::~RoutesFactory()
	{
		m_authorizationDataBuilder.reset();
		m_jwtParserService.reset();
		m_jsonAdapter.reset();

		m_unauthorizedReply.reset();
		m_forbiddenReply.reset();
	}

	void RoutesFactory::setUnauthorizedReply(const systelab::web_server::Reply& unauthorizedReply)
	{
		m_unauthorizedReply = std::make_unique<systelab::web_server::Reply>(unauthorizedReply.getStatus(), unauthorizedReply.getHeaders(), unauthorizedReply.getContent());
	}

	void RoutesFactory::setForbiddenReply(const systelab::web_server::Reply& forbiddenReply)
	{
		m_forbiddenReply = std::make_unique<systelab::web_server::Reply>(forbiddenReply.getStatus(), forbiddenReply.getHeaders(), forbiddenReply.getContent());
	}

	std::unique_ptr<IRoute>
	RoutesFactory::buildRoute(const std::string& method,
							  const std::string& uri,
							  const std::vector<RouteAccessValidatorFactoryMethod>& accessValidatorFactoryMethods,
							  EndpointFactoryMethod endpointFactoryMethod) const
	{
		return std::make_unique<Route>(method, uri, *m_authorizationDataBuilder,
									   accessValidatorFactoryMethods, endpointFactoryMethod,
									   *m_unauthorizedReply, *m_forbiddenReply);
	}

}}