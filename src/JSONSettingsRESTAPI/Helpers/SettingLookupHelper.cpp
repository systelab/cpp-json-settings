#include "stdafx.h"
#include "SettingLookupHelper.h"

#include "JSONSettings/SettingDefinitionMgr.h"


namespace systelab { namespace setting { namespace rest_api {

	boost::optional<SettingPath> SettingLookupHelper::findById(const SettingsFile& file, int settingId)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(file);
		for (const auto& settingData : settings)
		{
			if (settingData.second.id == settingId)
			{
				return settingData.first;
			}
		}

		return boost::none;
	}

}}}
