#pragma once

#include <string>


namespace systelab { namespace rest_api_core {

	class RouteFragment
	{
	public:
		RouteFragment(const std::string& item);
		virtual ~RouteFragment();

		bool isStringParameter() const;
		bool isNumericParameter() const;
		std::string getValue() const;

		bool match(const std::string& uriFragment) const;

	private:
		bool m_isStringParameter;
		bool m_isNumericParameter;
		std::string m_value;
	};

}}
