#pragma once

#include "JSONSettings/SettingsFile.h"
#include "JSONSettings/SettingPath.h"
#include "JSONSettings/SettingValue.h"


namespace systelab { namespace setting {
	class ISettingsService;
}}

namespace systelab { namespace setting { namespace rest_api {

	class SettingCurrentValueHelper
	{
	public:
		static SettingValue query(const ISettingsService&, const SettingsFile&, const SettingPath&, const SettingValueType&);
		static void set(ISettingsService&, const SettingsFile&, const SettingPath&, const SettingValue& newValue);
	};

}}}
