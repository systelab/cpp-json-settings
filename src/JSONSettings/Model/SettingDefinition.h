#pragma once

#include <functional>


namespace systelab { namespace setting {

	enum class SettingValueType
	{
		IntValue = 0,
		BooleanValue = 1,
		StringValue = 2
	};

	struct SettingValue
	{
		SettingValue()
			:type(SettingValueType::IntValue)
			,intValue(0)
		{
		}

		SettingValue(int value)
			:type(SettingValueType::IntValue)
			,intValue(value)
		{
		}

		SettingValue(bool value)
			:type(SettingValueType::BooleanValue)
			,boolValue(value)
		{
		}

		SettingValue(const std::string& value)
			:type(SettingValueType::StringValue)
			,stringValue(value)
		{
		}

		SettingValueType type;
		int intValue;
		bool boolValue;
		std::string stringValue;
	};

	struct SettingDefinition
	{
		bool cache;
		SettingValue defaultValue;
		std::function<bool(SettingValue)> validationFunction;
	};

}}
