#pragma once

#include "IRoute.h"

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Router/RouteFragment.h"


namespace systelab { namespace rest_api_core {

	class IRouteAccessValidator;

	class Route : public IRoute
	{
	public:
		Route(const std::string& method,
			  const std::string& uri,
			  const IRouteAccessValidator&,
			  std::function< std::unique_ptr<IEndpoint>() > factoryMethod);

		std::unique_ptr<systelab::web_server::Reply> execute(const systelab::web_server::Request&) const;

	protected:
		std::string m_method;
		std::vector<RouteFragment> m_fragments;
		const IRouteAccessValidator& m_accessValidator;
		std::function< std::unique_ptr<IEndpoint>() > m_factoryMethod;

	protected:
		std::vector<RouteFragment> buildFragmentsFromURI(const std::string& uri) const;
		bool validateMethod(const systelab::web_server::Request&) const;
		std::unique_ptr<EndpointRequestData> parseRequestData(const systelab::web_server::Request&) const;
	};

}}
