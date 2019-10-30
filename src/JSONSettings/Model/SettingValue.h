#pragma once


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
			:type(SettingValueType::StringValue)
			,value("")
		{
		}

		SettingValue(int intValue)
			:type(SettingValueType::IntValue)
		{
			std::stringstream ss;
			ss << intValue;
			value = ss.str();
		}

		SettingValue(bool boolValue)
			:type(SettingValueType::BooleanValue)
		{
			std::stringstream ss;
			ss << boolValue;
			value = ss.str();
		}

		SettingValue(const std::string& strValue)
			:type(SettingValueType::StringValue)
			,value(strValue)
		{
		}

		SettingValueType type;
		std::string value;
	};

}}
