#pragma once

#include "RESTAPICore/Router/IRoute.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockRoute : public IRoute
	{
	public:
		MockRoute();
		virtual ~MockRoute();

		MOCK_CONST_METHOD1(executeProxy, web_server::Reply* (const web_server::Request&));
		std::unique_ptr<web_server::Reply> execute(const web_server::Request& request) const
		{
			return std::unique_ptr<web_server::Reply>(executeProxy(request));
		}
	};

}}}

