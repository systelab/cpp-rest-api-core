#include "stdafx.h"
#include "FreeRouteAccessValidator.h"


namespace systelab { namespace rest_api_core {

	FreeRouteAccessValidator::FreeRouteAccessValidator()
	{
	}

	FreeRouteAccessValidator::~FreeRouteAccessValidator() = default;

	bool FreeRouteAccessValidator::hasAccess(EndpointRequestData&) const
	{
		return true;
	}

}}
