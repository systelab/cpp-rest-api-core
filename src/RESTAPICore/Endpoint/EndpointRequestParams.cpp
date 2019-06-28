#include "stdafx.h"
#include "EndpointRequestParams.h"


namespace systelab { namespace rest_api_core {

	EndpointRequestParams::EndpointRequestParams()
		:m_stringParameters()
		,m_numericParameters()
	{
	}
	
	EndpointRequestParams::~EndpointRequestParams() = default;


	// String parameters
	unsigned int EndpointRequestParams::getStringParameterCount() const
	{
		return (unsigned int) m_stringParameters.size();
	}

	std::vector<std::string> EndpointRequestParams::getStringParameterNames() const
	{
		std::vector<std::string> names;
		for (auto parameter : m_stringParameters)
		{
			names.push_back(parameter.first);
		}

		return names;
	}

	bool EndpointRequestParams::hasStringParameter(const std::string& name) const
	{
		return (m_stringParameters.find(name) != m_stringParameters.end());
	}

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

	void EndpointRequestParams::addStringParameter(const std::string& name, const std::string& value)
	{
		m_stringParameters.insert({ name, value });
	}


	// Numeric parameters
	unsigned int EndpointRequestParams::getNumericParameterCount() const
	{
		return (unsigned int) m_numericParameters.size();
	}

	std::vector<std::string> EndpointRequestParams::getNumericParameterNames() const
	{
		std::vector<std::string> names;
		for (auto parameter : m_numericParameters)
		{
			names.push_back(parameter.first);
		}

		return names;
	}

	bool EndpointRequestParams::hasNumericParameter(const std::string& name) const
	{
		return (m_numericParameters.find(name) != m_numericParameters.end());
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

	void EndpointRequestParams::addNumericParameter(const std::string& name, unsigned int value)
	{
		m_numericParameters.insert({ name, value });
	}

}}

