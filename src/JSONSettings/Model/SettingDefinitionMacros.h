#pragma once

#include "SettingDefinition.h"
#include "SettingDefinitionMgr.h"


#define NO_VALIDATION_FUNCTION() \
	[](SettingValue) -> bool { return true; }

#define JSON_SETTINGS_FILE(SETTINGS_FILE, CONTENT) \
	namespace SETTINGS_FILE \
	{ \
		static const std::string FILEPATH = SETTINGS_FILE; \
		CONTENT \
	}

#define JSON_SETTING_INT(SETTING_NAME, SETTING_PATH, SETTING_DEFAULT, USE_CACHE) \
	namespace SETTING_NAME \
	{ \
		static const std::string SETTING_PATH = #SETTING_PATH; \
		static const int DEFAULT_VALUE = SETTING_DEFAULT; \
		struct SETTING_NAME##SettingStruct \
		{ \
			SETTING_NAME##SettingStruct() \
			{ \
				SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				SettingDefinitionMgr::get().setSetting(FILEPATH, SETTING_PATH, settingDefinition); \
			} \
		}; \
		static SETTING_NAME##SettingStruct structInstance; \
	}


#define JSON_SETTING_INT_BOUNDED(SETTING_NAME, SETTING_PATH, SETTING_DEFAULT, MIN_BOUND, MAX_BOUND, USE_CACHE) \
	namespace SETTING_NAME \
	{ \
		static const std::string SETTING_PATH = #SETTING_PATH; \
		static const int DEFAULT_VALUE = SETTING_DEFAULT; \
		struct SETTING_NAME##SettingStruct \
		{ \
			SETTING_NAME##SettingStruct() \
			{ \
				SettingDefinition::ValidationFunction boundCheck = [](const SettingDefinition::SettingValue& v) -> bool \
																	{ \
																		bool aboveMinBound = (boost::get<int>(v) >= MIN_BOUND); \
																		bool belowMaxBound = (boost::get<int>(v) <= MAX_BOUND); \
																		return (aboveMinBound && belowMaxBound); \
																	}; \
				SettingDefinition settingDefinition = { DEFAULT_VALUE, boundCheck, USE_CACHE }; \
				SettingDefinitionMgr::get().setSetting(FILEPATH, SETTING_PATH, settingDefinition); \
			} \
		}; \
		static SETTING_NAME##SettingStruct structInstance; \
	}

#define JSON_SETTING_STRING(SETTING_NAME, SETTING_PATH, SETTING_DEFAULT, USE_CACHE) \
	namespace SETTING_NAME \
	{ \
		static const std::string SETTING_PATH = #SETTING_PATH; \
		static const std::string DEFAULT_VALUE = SETTING_DEFAULT; \
		struct SETTING_NAME##SettingStruct \
		{ \
			SETTING_NAME##SettingStruct() \
			{ \
				SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				SettingDefinitionMgr::get().setSetting(FILEPATH, SETTING_PATH, settingDefinition); \
			} \
		}; \
		static SETTING_NAME##SettingStruct structInstance; \
	}

#define JSON_SETTING_BOOL(SETTING_NAME, SETTING_PATH, SETTING_DEFAULT, USE_CACHE) \
	namespace SETTING_NAME \
	{ \
		static const std::string SETTING_PATH = #SETTING_PATH; \
		static const bool DEFAULT_VALUE = SETTING_DEFAULT; \
		struct SETTING_NAME##SettingStruct \
		{ \
			SETTING_NAME##SettingStruct() \
			{ \
				SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				SettingDefinitionMgr::get().setSetting(FILEPATH, SETTING_PATH, settingDefinition); \
			} \
		}; \
		static SETTING_NAME##SettingStruct structInstance; \
	}
