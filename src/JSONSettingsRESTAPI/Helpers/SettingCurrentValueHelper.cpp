#include "stdafx.h"
#include "SettingCurrentValueHelper.h"

#include "JSONSettings/ISettingsService.h"

#include <boost/lexical_cast.hpp>


namespace systelab { namespace setting { namespace rest_api {

	SettingValue SettingCurrentValueHelper::query(const ISettingsService& service,
												  const SettingsFile& file,
												  const SettingPath& path,
												  const SettingValueType& valueType)
	{
		SettingValue currentValue;
		if (valueType == SettingValueType::IntValue)
		{
			currentValue = SettingValue(service.getSettingInteger(file, path)).value;
		}
		else if (valueType == SettingValueType::DoubleValue)
		{
			currentValue = SettingValue(service.getSettingDouble(file, path)).value;
		}
		else if (valueType == SettingValueType::BooleanValue)
		{
			currentValue = SettingValue(service.getSettingBoolean(file, path)).value;
		}
		else if (valueType == SettingValueType::StringValue)
		{
			currentValue = SettingValue(service.getSettingString(file, path)).value;
		}

		return currentValue;
	}

	void SettingCurrentValueHelper::set(ISettingsService& service,
										const SettingsFile& file,
										const SettingPath& path,
										const SettingValue& newValue)
	{
		SettingValueType valueType = newValue.type;
		if (valueType == SettingValueType::IntValue)
		{
			int newValueInt = boost::lexical_cast<int>(newValue.value);
			service.setSettingInteger(file, path, newValueInt);
		}
		else if (valueType == SettingValueType::DoubleValue)
		{
			double newValueDbl = boost::lexical_cast<double>(newValue.value);
			service.setSettingDouble(file, path, newValueDbl);
		}
		else if (valueType == SettingValueType::BooleanValue)
		{
			bool newValueBool = (newValue.value == "true");
			service.setSettingBoolean(file, path, newValueBool);
		}
		else if (valueType == SettingValueType::StringValue)
		{
			std::string newValueStr = newValue.value;
			service.setSettingString(file, path, newValueStr);
		}
	}

}}}
