#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace rest_api_core {

	class IRoute;

	class Router
	{
	public:
		Router();
		virtual ~Router();

		void addRoute(std::unique_ptr<IRoute>);

		std::unique_ptr<systelab::web_server::Reply> process(const systelab::web_server::Request& request) const;

	private:
		std::vector< std::unique_ptr<IRoute> > m_routes;
	};

}}
