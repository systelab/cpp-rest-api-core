#pragma once

#include <memory>


namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace rest_api_core {
	
	class IRoute
	{
	public:
		virtual ~IRoute() = default;

		virtual std::unique_ptr<systelab::web_server::Reply> execute(const systelab::web_server::Request&) const = 0;
	};

}}
