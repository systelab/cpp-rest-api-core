#include "stdafx.h"
#include "EndpointRequestParams.h"


namespace systelab { namespace rest_api_core {

	EndpointRequestParams::EndpointRequestParams()
		:m_stringParameters()
		,m_numericParameters()
	{
	}
	
	EndpointRequestParams::~EndpointRequestParams() = default;

	std::string EndpointRequestParams::getStringParameter(const std::string& name) const
	{
		auto it = m_stringParameters.find(name);
		if (it != m_stringParameters.end())
		{
			return it->second;
		}
		else
		{
			throw std::runtime_error("String parameter '" + name + "' not found");
		}
	}

	unsigned int EndpointRequestParams::getNumericParameter(const std::string& name) const
	{
		auto it = m_numericParameters.find(name);
		if (it != m_numericParameters.end())
		{
			return it->second;
		}
		else
		{
			throw std::runtime_error("Numeric parameter '" + name + "' not found");
		}
	}

	void EndpointRequestParams::addStringParameter(const std::string& name, const std::string& value)
	{
		m_stringParameters.insert({ name, value });
	}

	void EndpointRequestParams::addNumericParameter(const std::string& name, unsigned int value)
	{
		m_numericParameters.insert({ name, value });
	}

}}

