#pragma once

#include "JSONSettings/Model/SettingDefinitionMacros.h"


#define CACHE_ENABLED true
#define CACHE_DISABLED false

namespace systelab { namespace setting { namespace unit_test {

	JSON_SETTINGS_FILE(MySettingsFile, "MySettingsFile.json",

		// Root settings
		JSON_SETTING_INT (RootIntSettingCache,  "IntSettingCache",  1234, CACHE_ENABLED)
		JSON_SETTING_DBL (RootDblSettingCache,  "DblSettingCache", 1.234, CACHE_ENABLED)
		JSON_SETTING_STR (RootStrSettingCache,  "StrSettingCache",  "ab", CACHE_ENABLED)
		JSON_SETTING_BOOL(RootBoolSettingCache, "BoolSettingCache", true, CACHE_ENABLED)

		JSON_SETTING_INT (RootIntSettingNoCache,  "IntSettingNoCache",   5678, CACHE_DISABLED)
		JSON_SETTING_DBL (RootDblSettingNoCache,  "DblSettingNoCache",  5.678, CACHE_DISABLED)
		JSON_SETTING_STR (RootStrSettingNoCache,  "StrSettingNoCache",   "cd", CACHE_DISABLED)
		JSON_SETTING_BOOL(RootBoolSettingNoCache, "BoolSettingNoCache", false, CACHE_DISABLED)

		// Section settings
		JSON_SETTING_INT (SectionIntSetting,  "Section.IntSettingCache",  4321, CACHE_ENABLED)
		JSON_SETTING_DBL (SectionDblSetting,  "Section.DblSettingCache", 4.321, CACHE_ENABLED)
		JSON_SETTING_STR (SectionStrSetting,  "Section.StrSettingCache",  "ba", CACHE_ENABLED)
		JSON_SETTING_BOOL(SectionBoolSetting, "Section.BoolSettingCache", true, CACHE_ENABLED)

		// Subsection settings
		JSON_SETTING_INT (SubsectionIntSetting,  "Section.Subsection.IntSettingCache",   8765, CACHE_DISABLED)
		JSON_SETTING_DBL (SubsectionDblSetting,  "Section.Subsection.DblSettingCache",  8.765, CACHE_DISABLED)
		JSON_SETTING_STR (SubsectionStrSetting,  "Section.Subsection.StrSettingCache",   "dc", CACHE_DISABLED)
		JSON_SETTING_BOOL(SubsectionBoolSetting, "Section.Subsection.BoolSettingCache", false, CACHE_DISABLED)
	);

	JSON_SETTINGS_FILE(SecondSettingsFile, "SecondSettingsFile.json",
		JSON_SETTING_INT (RootIntSettingCache,  "IntSettingCache",      2222, CACHE_ENABLED)
		JSON_SETTING_DBL (RootDblSettingCache,  "DblSettingCache",     2.222, CACHE_ENABLED)
		JSON_SETTING_STR (RootStrSettingCache,  "StrSettingCache",  "SECOND", CACHE_ENABLED)
		JSON_SETTING_BOOL(RootBoolSettingCache, "BoolSettingCache",    false, CACHE_ENABLED)
	);
	
	JSON_SETTINGS_ENCRYPTED_FILE(EncryptedSettingsFile, "EncryptedSettingsFile.json", []() { return "Key"; },
		JSON_SETTING_INT(RootIntSettingCache,   "IntSettingCache",  9999, CACHE_ENABLED)
		JSON_SETTING_DBL(RootDblSettingCache,   "DblSettingCache",   9.9, CACHE_ENABLED)
		JSON_SETTING_STR(RootStrSettingCache,   "StrSettingCache",  "99", CACHE_ENABLED)
		JSON_SETTING_BOOL(RootBoolSettingCache, "BoolSettingCache", true, CACHE_ENABLED)
	);

	JSON_SETTINGS_FILE(InvalidSettingsFile, "<|?s/>",
		JSON_SETTING_INT (IntSetting,  "IntSetting",   111, CACHE_ENABLED)
		JSON_SETTING_DBL (DblSetting,  "DblSetting",   1.1, CACHE_ENABLED)
		JSON_SETTING_STR (StrSetting,  "StrSetting",   "A", CACHE_ENABLED)
		JSON_SETTING_BOOL(BoolSetting, "BoolSetting", true, CACHE_ENABLED)
	);

}}}

