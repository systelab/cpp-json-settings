#pragma once


namespace systelab { namespace setting {

	enum class SettingValueType
	{
		IntValue = 0,
		DoubleValue = 1,
		BooleanValue = 2,
		StringValue = 3
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

		SettingValue(double doubleValue)
			:type(SettingValueType::DoubleValue)
		{
			std::stringstream ss;
			ss << std::setprecision(std::numeric_limits<double>::max_digits10 - 1) << doubleValue;
			value = ss.str();
		}

		SettingValue(bool boolValue)
			:type(SettingValueType::BooleanValue)
			,value(boolValue ? "true" : "false")
		{
		}

		SettingValue(const std::string& strValue)
			:type(SettingValueType::StringValue)
			,value(strValue)
		{
		}

		inline friend bool operator== (const SettingValue& lhs, const SettingValue& rhs)
		{
			return (lhs.type == rhs.type) && (lhs.value == rhs.value);
		}

		SettingValueType type;
		std::string value;
	};

}}
