#include "stdafx.h"
#include "SettingCurrentValueHelper.h"

#include "JSONSettings/ISettingsService.h"


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

}}}
