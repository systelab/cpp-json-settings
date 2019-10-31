#pragma once

#include "JSONSettings/Model/SettingDefinitionMacros.h"


#define CACHE_ENABLED true
#define CACHE_DISABLED false

namespace systelab { namespace setting { namespace unit_test {

	JSON_SETTINGS_FILE(MySettingsFile, "MySettingsFile.json",

		// Root settings
		JSON_SETTING_INT (RootIntSettingCache,  "IntSettingCache",  1234, CACHE_ENABLED)
		JSON_SETTING_STR (RootStrSettingCache,  "StrSettingCache",  "ab", CACHE_ENABLED)
		JSON_SETTING_BOOL(RootBoolSettingCache, "BoolSettingCache", true, CACHE_ENABLED)

		JSON_SETTING_INT (RootIntSettingNoCache,  "IntSettingNoCache",   5678, CACHE_DISABLED)
		JSON_SETTING_STR (RootStrSettingNoCache,  "StrSettingNoCache",   "cd", CACHE_DISABLED)
		JSON_SETTING_BOOL(RootBoolSettingNoCache, "BoolSettingNoCache", false, CACHE_DISABLED)

		// Section settings
		JSON_SETTING_INT (SectionIntSetting,  "Section.IntSettingCache",  4321, CACHE_ENABLED)
		JSON_SETTING_STR (SectionStrSetting,  "Section.StrSettingCache",  "ba", CACHE_ENABLED)
		JSON_SETTING_BOOL(SectionBoolSetting, "Section.BoolSettingCache", true, CACHE_ENABLED)

		// Subsection settings
		JSON_SETTING_INT (SubsectionIntSetting,  "Section.Subsection.IntSettingCache",  8765,  CACHE_DISABLED)
		JSON_SETTING_STR (SubsectionStrSetting,  "Section.Subsection.StrSettingCache",  "dc",  CACHE_DISABLED)
		JSON_SETTING_BOOL(SubsectionBoolSetting, "Section.Subsection.BoolSettingCache", false, CACHE_DISABLED)
	);

}}}

