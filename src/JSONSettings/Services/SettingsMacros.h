#pragma once

#define GET_JSON_SETTING_INT(SERVICE, FILENAMESPACE, SETTINGNAMESPACE) \
	(SERVICE).getSettingInteger(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH)

#define GET_JSON_SETTING_STR(SERVICE, FILENAMESPACE, SETTINGNAMESPACE) \
	(SERVICE).getSettingString(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH)

#define GET_JSON_SETTING_BOOL(SERVICE, FILENAMESPACE, SETTINGNAMESPACE) \
	(SERVICE).getSettingBoolean(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH)


#define SET_JSON_SETTING_INT(SERVICE, FILENAMESPACE, SETTINGNAMESPACE, SETTING_VALUE) \
	(SERVICE).setSettingInteger(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH, SETTING_VALUE)

#define SET_JSON_SETTING_STR(SERVICE, FILENAMESPACE, SETTINGNAMESPACE, SETTING_VALUE) \
	(SERVICE).setSettingString(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH, SETTING_VALUE)

#define SET_JSON_SETTING_BOOL(SERVICE, FILENAMESPACE, SETTINGNAMESPACE, SETTING_VALUE) \
	(SERVICE).setSettingBoolean(FILENAMESPACE::FILENAME, FILENAMESPACE::SETTINGNAMESPACE::PATH, SETTING_VALUE)

