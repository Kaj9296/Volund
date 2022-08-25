#pragma once

#include <json/single_include/nlohmann/json.hpp>

#include "Logger/Logger.h"

namespace Volund
{
	class JSON
	{
	public:

		uint64_t Size();

		template<typename T>
		T GetAs();

		template<typename T>
		bool Contains(T const& Entry);

		JSON operator[](int32_t const& Other);
		JSON operator[](const char* Other);

		template<typename T>
		operator T();

		template<typename T>
		bool operator==(T const& Other)
		{
			return this->_JSONObject == Other;
		}

		static JSON Load(std::string const& FilePath);

		JSON() = default;

		JSON(nlohmann::json const& JSONObject);

	private:

		nlohmann::json _JSONObject;
	};

	template<typename T>
	T JSON::GetAs()
	{
		return this->_JSONObject.get<T>();
	}
	  
	template<typename T>
	bool JSON::Contains(T const& Entry)
	{
		return this->_JSONObject.contains(Entry);
	}

	template<typename T>
	JSON::operator T()
	{
		return this->GetAs<T>();
	}		
}