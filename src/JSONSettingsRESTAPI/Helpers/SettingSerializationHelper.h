#pragma once

#include "JSONSettings/SettingDefinition.h"
#include "JSONSettings/SettingPath.h"
#include "JSONSettings/SettingValue.h"

#include "JSONAdapterInterface/IJSONValue.h"


namespace systelab { namespace setting { namespace rest_api {

	class SettingSerializationHelper
	{
	public:
		static void serialize(const SettingPath& path,
							  const SettingDefinition& definition,
							  const SettingValue& currentValue,
							  systelab::json::IJSONValue& jsonValue);

		static std::string translateSettingValueTypeToString(const SettingValueType&);
	};

}}}
