#pragma once

#include "SettingDefinition.h"
#include "SettingDefinitionMgr.h"


#define NO_VALIDATION_FUNCTION() \
	[](systelab::setting::SettingValue) -> bool { return true; }

#define SET_JSON_SETTINGS_FOLDER(FOLDER) \
	systelab::setting::SettingDefinitionMgr().get().setSettingsFolderPath(FOLDER);


#define JSON_SETTINGS_FILE(SETTINGS_FILE_NAMESPACE, SETTINGS_FILENAME, CONTENT) \
	namespace SETTINGS_FILE_NAMESPACE \
	{ \
		static const std::string FILENAME = SETTINGS_FILENAME; \
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
				systelab::setting::SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				systelab::setting::SettingDefinitionMgr::get().setSetting(FILENAME, SETTING_PATH, settingDefinition); \
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
				systelab::setting::SettingDefinition::ValidationFunction boundCheck = [](const SettingDefinition::SettingValue& v) -> bool \
																	{ \
																		bool aboveMinBound = (boost::get<int>(v) >= MIN_BOUND); \
																		bool belowMaxBound = (boost::get<int>(v) <= MAX_BOUND); \
																		return (aboveMinBound && belowMaxBound); \
																	}; \
				systelab::setting::SettingDefinition settingDefinition = { DEFAULT_VALUE, boundCheck, USE_CACHE }; \
				systelab::setting::SettingDefinitionMgr::get().setSetting(FILENAME, SETTING_PATH, settingDefinition); \
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
				systelab::setting::SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				systelab::setting::SettingDefinitionMgr::get().setSetting(FILENAME, SETTING_PATH, settingDefinition); \
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
				systelab::setting::SettingDefinition settingDefinition = { DEFAULT_VALUE, NO_VALIDATION_FUNCTION, USE_CACHE }; \
				systelab::setting::SettingDefinitionMgr::get().setSetting(FILENAME, SETTING_PATH, settingDefinition); \
			} \
		}; \
		static SETTING_NAME##SettingStruct structInstance; \
	}
