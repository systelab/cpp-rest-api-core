#pragma once

#include "IRouteAccessValidator.h"


namespace systelab { namespace rest_api_core {

	class FreeRouteAccessValidator : public IRouteAccessValidator
	{
	public:
		FreeRouteAccessValidator();
		virtual ~FreeRouteAccessValidator();

		bool hasAccess(EndpointRequestData&) const override;
	};

}}
