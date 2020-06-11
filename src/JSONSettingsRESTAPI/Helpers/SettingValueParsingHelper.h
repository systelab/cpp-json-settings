#pragma once

#include "JSONSettings/SettingValue.h"

#include <boost/optional.hpp>


namespace systelab { namespace json {
	class IJSONAdapter;
}}

namespace systelab { namespace setting { namespace rest_api {

	class SettingValueParsingHelper
	{
	public:
		static boost::optional<SettingValue> parse(const systelab::json::IJSONAdapter&,
												   const SettingValueType&,
												   const std::string& requestContent,
												   std::string& errorMessage);
	};

}}}
