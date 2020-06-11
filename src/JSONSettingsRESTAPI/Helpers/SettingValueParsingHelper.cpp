#include "stdafx.h"
#include "SettingValueParsingHelper.h"

#include "JSONAdapterInterface/IJSONAdapter.h"


namespace systelab { namespace setting { namespace rest_api {

	boost::optional<SettingValue> SettingValueParsingHelper::parse(const systelab::json::IJSONAdapter& jsonAdapter,
																   const SettingValueType& valueType,
																   const std::string& requestContent,
																   std::string& errorMessage)
	{
		return boost::none;
	}

}}}
