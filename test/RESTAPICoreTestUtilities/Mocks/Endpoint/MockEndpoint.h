#pragma once

#include "RESTAPICore/Endpoint/IEndpoint.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"

#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class MockEndpoint : public IEndpoint
	{
	public:
		MockEndpoint();
		virtual ~MockEndpoint();

		MOCK_METHOD1(executeProxy, web_server::Reply* (const EndpointRequestData&));
		std::unique_ptr<web_server::Reply> execute(const EndpointRequestData& endpointRequestData)
		{
			return std::unique_ptr<web_server::Reply>(executeProxy(endpointRequestData));
		}
	};

}}}

