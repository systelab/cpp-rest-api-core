#pragma once

#include <string>
#include <vector>


namespace systelab { namespace rest_api_core {
	
	class IUserRoleService
	{
	public:
		virtual ~IUserRoleService() = default;

		virtual std::vector<std::string> getUserRoles(const std::string& username) const = 0;
	};

}}
