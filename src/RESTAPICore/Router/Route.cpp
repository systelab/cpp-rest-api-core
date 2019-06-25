#include "stdafx.h"
#include "Route.h"

#include "RouteAccess/IRouteAccessValidator.h"
#include "Router/IAuthorizationDataBuilder.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Headers.h"

#include <sstream>


namespace systelab { namespace rest_api_core {

	Route::Route(const std::string& method,
				 const std::string& uri,
				 const IAuthorizationDataBuilder& authorizationDataBuilder,
				 const std::vector<RouteAccessValidatorFactoryMethod>& accessValidatorFactoryMethods,
				 EndpointFactoryMethod endpointFactoryMethod)
		:m_method(method)
		,m_fragments(buildFragmentsFromURI(uri))
		,m_authorizationDataBuilder(authorizationDataBuilder)
		,m_accessValidatorFactoryMethods(accessValidatorFactoryMethods)
		,m_endpointFactoryMethod(endpointFactoryMethod)
	{
	}

	std::unique_ptr<systelab::web_server::Reply> Route::execute(const systelab::web_server::Request& request) const
	{
		if (!validateMethod(request))
		{
			return nullptr;
		}

		auto requestData = parseRequestData(request);
		if (!requestData)
		{
			return nullptr;
		}

		for (auto& accessValidatorFactoryMethod : m_accessValidatorFactoryMethods)
		{
			auto accessValidator = accessValidatorFactoryMethod();
			if (!accessValidator->hasAccess(*requestData))
			{
				std::map<std::string, std::string> headers = { {std::string("Content-Type"), std::string("application/json")} };
				return std::make_unique<systelab::web_server::Reply>(systelab::web_server::Reply::FORBIDDEN, headers, "{}");
			}
		}

		try
		{
			std::unique_ptr<IEndpoint> endpoint = m_endpointFactoryMethod();
			if (endpoint)
			{
				return endpoint->execute(*requestData);
			}
		}
		catch (std::exception&)
		{
		}

		return nullptr;
	}

	bool Route::validateMethod(const systelab::web_server::Request& request) const
	{
		return (request.getMethod() == m_method);
	}

	std::unique_ptr<EndpointRequestData> Route::parseRequestData(const systelab::web_server::Request& request) const
	{
		std::vector<RouteFragment> requestFragments = buildFragmentsFromURI(request.getURI());
		if (requestFragments.size() != m_fragments.size())
		{
			return nullptr;
		}

		EndpointRequestParams requestParams;
		unsigned int nFragments = m_fragments.size();
		for (unsigned int i = 0; i < nFragments; i++)
		{
			const auto& requestFragment = requestFragments[i];
			if (requestFragment.isNumericParameter() || requestFragment.isStringParameter())
			{
				return nullptr;
			}

			const auto& fragment = m_fragments[i];
			if (!fragment.match(requestFragment.getValue()))
			{
				return nullptr;
			}

			if (fragment.isNumericParameter())
			{
				unsigned int numericValue = static_cast<unsigned int>(std::atoi(requestFragment.getValue().c_str()));
				requestParams.addNumericParameter(fragment.getValue(), numericValue);
			}
			else if (fragment.isStringParameter())
			{
				requestParams.addStringParameter(fragment.getValue(), requestFragment.getValue());
			}
		}

		auto authorizationData = m_authorizationDataBuilder.buildAuthorizationData(request.getHeaders());

		return std::make_unique<EndpointRequestData>(requestParams, request.getContent(),
													 request.getHeaders(), request.getQueryStrings(),
													 *authorizationData);
	}

	std::vector<RouteFragment> Route::buildFragmentsFromURI(const std::string& uri) const
	{
		std::vector<RouteFragment> fragments;

		std::istringstream iss(uri);
		std::string item;

		while (std::getline(iss, item, '/'))
		{
			if (item.empty())
			{
				continue;
			}

			fragments.push_back(RouteFragment(item));
		}

		return fragments;
	}

}}