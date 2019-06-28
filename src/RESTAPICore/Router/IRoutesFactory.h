#pragma once

namespace systelab { namespace rest_api_core {

	class IEndpoint;
	class IRoute;
	class IRouteAccessValidator;

	class IRoutesFactory
	{
	public:
		typedef std::function< std::unique_ptr<IRouteAccessValidator>() > RouteAccessValidatorFactoryMethod;
		typedef std::function< std::unique_ptr<IEndpoint>() > EndpointFactoryMethod;

	public:
		virtual ~IRoutesFactory() = default;

		virtual std::unique_ptr<IRoute>
		buildRoute(const std::string& method,
				   const std::string& uri,
				   const std::vector<RouteAccessValidatorFactoryMethod>&,
				   EndpointFactoryMethod) const = 0;
	};

}}
