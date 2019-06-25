#pragma once

#include "IRoute.h"

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Router/RouteFragment.h"


namespace systelab { namespace rest_api_core {

	class IAuthorizationDataBuilder;
	class IRouteAccessValidator;

	class Route : public IRoute
	{
	public:
		typedef std::function< std::unique_ptr<IRouteAccessValidator>() > RouteAccessValidatorFactoryMethod;
		typedef std::function< std::unique_ptr<IEndpoint>() > EndpointFactoryMethod;

	public:
		Route(const std::string& method,
			  const std::string& uri,
			  const IAuthorizationDataBuilder&,
			  const std::vector<RouteAccessValidatorFactoryMethod>&,
			  EndpointFactoryMethod factoryMethod);

		std::unique_ptr<systelab::web_server::Reply> execute(const systelab::web_server::Request&) const;

	protected:
		std::string m_method;
		std::vector<RouteFragment> m_fragments;
		const IAuthorizationDataBuilder& m_authorizationDataBuilder;
		std::vector<RouteAccessValidatorFactoryMethod> m_accessValidatorFactoryMethods;
		EndpointFactoryMethod m_endpointFactoryMethod;

	protected:
		bool validateMethod(const systelab::web_server::Request&) const;
		std::unique_ptr<EndpointRequestData> parseRequestData(const systelab::web_server::Request&) const;
		std::vector<RouteFragment> buildFragmentsFromURI(const std::string& uri) const;
	};

}}
