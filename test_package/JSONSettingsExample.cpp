#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>

#include "JSONSettings/SettingsService.h"
#include "JSONSettings/SettingDefinitionMacros.h"
#include "JSONSettings/SettingsMacros.h"


JSON_SETTINGS_FILE(TestSettingsFile, "PackageSettingsFile.json",
	JSON_SETTING_INT(TestSetting, "test.setting.path", 789, false)
)

int main(int argc, char *argv[])
{
	int settingDefaultValue = GET_JSON_SETTING_INT(systelab::setting::SettingsService(), TestSettingsFile, TestSetting);
	std::cout << "Setting default value: " << settingDefaultValue << std::endl;

    return 0;
}
