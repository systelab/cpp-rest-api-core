#pragma once

#include "RESTAPICore/Endpoint/EndpointRequestParams.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	class EndpointRequestParamsBuilder
	{
	public:
		EndpointRequestParamsBuilder();
		virtual ~EndpointRequestParamsBuilder();

		EndpointRequestParamsBuilder& addStringParameter(const std::string& name, const std::string& value);
		EndpointRequestParamsBuilder& addNumericParameter(const std::string& name, unsigned int value);

		EndpointRequestParams getEntity() const;

	private:
		EndpointRequestParams m_entity;
	};

}}}

