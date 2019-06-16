#pragma once


namespace systelab { namespace rest_api_core {

	class EndpointRequestParams
	{
	public:
		EndpointRequestParams();
		virtual ~EndpointRequestParams();

		std::string getStringParameter(const std::string& name) const;
		unsigned int getNumericParameter(const std::string& name) const;

		void addStringParameter(const std::string& name, const std::string& value);
		void addNumericParameter(const std::string& name, unsigned int value);

	private:
		std::map<std::string, std::string> m_stringParameters;
		std::map<std::string, unsigned int> m_numericParameters;
	};

}}

