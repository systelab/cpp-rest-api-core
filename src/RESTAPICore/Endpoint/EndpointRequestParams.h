#pragma once


namespace systelab { namespace rest_api_core {

	class EndpointRequestParams
	{
	public:
		EndpointRequestParams();
		EndpointRequestParams(const EndpointRequestParams&);
		EndpointRequestParams(const std::map<std::string, std::string>&,
							  const std::map<std::string, unsigned int>&);
		virtual ~EndpointRequestParams();

		// String parameters
		unsigned int getStringParameterCount() const;
		std::vector<std::string> getStringParameterNames() const;
		bool hasStringParameter(const std::string& name) const;
		std::string getStringParameter(const std::string& name) const;
		void addStringParameter(const std::string& name, const std::string& value);

		// Numeric parameters
		unsigned int getNumericParameterCount() const;
		std::vector<std::string> getNumericParameterNames() const;
		bool hasNumericParameter(const std::string& name) const;
		unsigned int getNumericParameter(const std::string& name) const;
		void addNumericParameter(const std::string& name, unsigned int value);

		EndpointRequestParams& operator= (const EndpointRequestParams&);
		friend bool operator== (const EndpointRequestParams&, const EndpointRequestParams&);
		friend bool operator!= (const EndpointRequestParams&, const EndpointRequestParams&);

	private:
		std::map<std::string, std::string> m_stringParameters;
		std::map<std::string, unsigned int> m_numericParameters;
	};

}}

