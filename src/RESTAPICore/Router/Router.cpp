#include "stdafx.h"
#include "Router.h"

#include "Router/Route.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"


namespace systelab { namespace rest_api_core {

	Router::Router()
	{
	}

	Router::~Router() = default;

	void Router::addRoute(std::unique_ptr<IRoute> route)
	{
		m_routes.push_back(std::move(route));
	}

	std::unique_ptr<systelab::web_server::Reply> Router::process(const systelab::web_server::Request& request) const
	{
		for (const auto& route : m_routes)
		{
			std::unique_ptr<systelab::web_server::Reply> reply = route->execute(request);
			if (reply)
			{
				return std::move(reply);
			}
		}

		return std::unique_ptr<systelab::web_server::Reply>();
	}

}}
