#pragma once


namespace systelab { namespace rest_api_core {

	class EndpointRequestData;

	class IRouteAccessValidator
	{
	public:
		virtual ~IRouteAccessValidator() {}

		virtual bool hasAccess(EndpointRequestData&) const = 0;
	};

}}
