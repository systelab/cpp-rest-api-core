#include "stdafx.h"
#include "RouteFragment.h"

#include <cctype>


namespace systelab { namespace rest_api_core {

	RouteFragment::RouteFragment(const std::string& fragment)
	{
		m_isStringParameter = (fragment[0] == ':');
		m_isNumericParameter = (fragment[0] == '+');
		m_value = (m_isStringParameter || m_isNumericParameter) ? fragment.substr(1) : fragment;
	}

	RouteFragment::~RouteFragment() = default;

	bool RouteFragment::isStringParameter() const
	{
		return m_isStringParameter;
	}

	bool RouteFragment::isNumericParameter() const
	{
		return m_isNumericParameter;
	}

	std::string RouteFragment::getValue() const
	{
		return m_value;
	}

	bool RouteFragment::match(const std::string& uriFragment) const
	{
		if (m_isStringParameter)
		{
			return true;
		}
		else if (m_isNumericParameter)
		{
			std::string::const_iterator it = uriFragment.begin();
			while (it != uriFragment.end() && std::isdigit(*it)) ++it;
			return !uriFragment.empty() && it == uriFragment.end();
		}
		else
		{
			return (uriFragment == m_value);
		}
	}

}}
