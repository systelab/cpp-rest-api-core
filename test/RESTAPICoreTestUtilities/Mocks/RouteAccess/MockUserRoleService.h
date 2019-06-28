#pragma once

#include "RESTAPICore/RouteAccess/IUserRoleService.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockUserRoleService : public IUserRoleService
	{
	public:
		MockUserRoleService();
		virtual ~MockUserRoleService();

		MOCK_CONST_METHOD1(getUserRoles, std::vector<std::string>(const std::string&));
	};

}}}

