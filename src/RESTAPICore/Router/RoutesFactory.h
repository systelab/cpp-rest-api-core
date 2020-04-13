#pragma once

#include "IRoutesFactory.h"


namespace systelab {
	namespace json {
		class IJSONAdapter;
	}
	namespace jwt {
		class ITokenParserService;
	}
	namespace web_server {
		class Reply;
	}
}

namespace systelab { namespace rest_api_core {

	class IAuthorizationDataBuilder;
	class IRouteAccessValidator;

	class RoutesFactory : public IRoutesFactory
	{
	public:
		RoutesFactory(const std::string& jwtKey);
		virtual ~RoutesFactory();

		void setUnauthorizedReply(const systelab::web_server::Reply&);
		void setForbiddenReply(const systelab::web_server::Reply&);

		std::unique_ptr<IRoute>
		buildRoute(const std::string& method,
				   const std::string& uri,
				   const std::vector<RouteAccessValidatorFactoryMethod>&,
				   EndpointFactoryMethod) const override;

	protected:
		std::unique_ptr<json::IJSONAdapter> m_jsonAdapter;
		std::unique_ptr<jwt::ITokenParserService> m_jwtParserService;
		std::unique_ptr<IAuthorizationDataBuilder> m_authorizationDataBuilder;

		std::unique_ptr<systelab::web_server::Reply> m_unauthorizedReply;
		std::unique_ptr<systelab::web_server::Reply> m_forbiddenReply;
	};

}}
