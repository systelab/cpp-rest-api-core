#include "stdafx.h"
#include "EndpointRequestParamsBuilder.h"


namespace systelab { namespace rest_api_core { namespace test_utility {

	EndpointRequestParamsBuilder::EndpointRequestParamsBuilder()
		:m_entity()
	{
	}

	EndpointRequestParamsBuilder::~EndpointRequestParamsBuilder() = default;

	EndpointRequestParamsBuilder& EndpointRequestParamsBuilder::addStringParameter(const std::string& name, const std::string& value)
	{
		m_entity.addStringParameter(name, value);
		return *this;
	}

	EndpointRequestParamsBuilder& EndpointRequestParamsBuilder::addNumericParameter(const std::string& name, unsigned int value)
	{
		m_entity.addNumericParameter(name, value);
		return *this;
	}

	EndpointRequestParams EndpointRequestParamsBuilder::getEntity() const
	{
		return m_entity;
	}

}}}
