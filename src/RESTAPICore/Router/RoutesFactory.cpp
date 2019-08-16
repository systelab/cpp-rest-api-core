#include "stdafx.h"
#include "RoutesFactory.h"

#include "Router/AuthorizationDataBuilder.h"
#include "Router/Route.h"

#include "JWTUtils/Services/TokenParserService.h"

#include "RapidJSONAdapter/JSONAdapter.h"


namespace systelab { namespace rest_api_core {

	RoutesFactory::RoutesFactory(const std::string& jwtKey)
		:m_jsonAdapter(std::make_unique<json::rapidjson::JSONAdapter>())
		,m_jwtParserService(std::make_unique<jwt::TokenParserService>(*m_jsonAdapter))
		,m_authorizationDataBuilder(std::make_unique<AuthorizationDataBuilder>(jwtKey, *m_jwtParserService))
	{
	}

	RoutesFactory::~RoutesFactory()
	{
		m_authorizationDataBuilder.reset();
		m_jwtParserService.reset();
		m_jsonAdapter.reset();
	}

	std::unique_ptr<IRoute>
	RoutesFactory::buildRoute(const std::string& method,
							  const std::string& uri,
							  const std::vector<RouteAccessValidatorFactoryMethod>& accessValidatorFactoryMethods,
							  EndpointFactoryMethod endpointFactoryMethod) const
	{
		return std::make_unique<Route>(method, uri, *m_authorizationDataBuilder,
									   accessValidatorFactoryMethods, endpointFactoryMethod);
	}

}}