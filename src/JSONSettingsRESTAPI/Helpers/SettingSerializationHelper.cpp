﻿#include "stdafx.h"
#include "SettingSerializationHelper.h"


namespace systelab { namespace setting { namespace rest_api {

	void SettingSerializationHelper::serialize(const SettingPath& path,
											   const SettingDefinition& definition,
											   const SettingValue& currentValue,
											   systelab::json::IJSONValue& jsonValue)
	{
		jsonValue.addMember("id", definition.id);
		jsonValue.addMember("path", path);
		jsonValue.addMember("type", translateSettingValueTypeToString(definition.defaultValue.type));
		jsonValue.addMember("useCache", definition.useCache);
		jsonValue.addMember("defaultValue", definition.defaultValue.value);
		jsonValue.addMember("currentValue", currentValue.value);
	}

	std::string SettingSerializationHelper::translateSettingValueTypeToString(const SettingValueType& type)
	{
		std::string typeStr = "";
		if (type == SettingValueType::BooleanValue)
		{
			typeStr = "boolean";
		}
		else if (type == SettingValueType::DoubleValue)
		{
			typeStr = "double";
		}
		else if (type == SettingValueType::IntValue)
		{
			typeStr = "integer";
		}
		else if (type == SettingValueType::StringValue)
		{
			typeStr = "string";
		}

		return typeStr;
	}

}}}
