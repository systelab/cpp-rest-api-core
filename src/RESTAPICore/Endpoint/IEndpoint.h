#pragma once

#include "EndpointRequestData.h"


namespace systelab { namespace web_server {
	class Reply;
}}

namespace systelab { namespace rest_api_core {

	class IEndpoint
	{
	public:
		virtual ~IEndpoint() {};

		virtual std::unique_ptr<systelab::web_server::Reply> execute(const EndpointRequestData&) = 0;
	};

}}

