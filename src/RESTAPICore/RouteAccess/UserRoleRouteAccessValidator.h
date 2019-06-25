#pragma once

#include "IRouteAccessValidator.h"

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Router/RouteFragment.h"

namespace systelab { namespace jwt {
	class ITokenParserService;
}}

namespace systelab { namespace rest_api_core {

	class IUserRoleService;

	class UserRoleRouteAccessValidator : public IRouteAccessValidator
	{
	public:
		UserRoleRouteAccessValidator(const std::vector<std::string>& allowedRoles,
									 const IUserRoleService&);
		virtual ~UserRoleRouteAccessValidator();

		bool hasAccess(EndpointRequestData&) const override;

	private:
		std::vector<std::string> m_allowedRoles;
		const IUserRoleService& m_userRoleService;
	};

}}
