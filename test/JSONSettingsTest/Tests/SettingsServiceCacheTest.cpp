#include "stdafx.h"
#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"

#include <boost/filesystem.hpp>


using namespace testing;

namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceCacheTest : public ::testing::Test
	{
		void SetUp()
		{
			auto folderPath = boost::filesystem::path("SettingsServiceNoCacheTest");
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			SET_JSON_SETTINGS_FOLDER(boost::filesystem::absolute(folderPath).string());

			std::stringstream ss;
			ss << "{" << std::endl;
			ss << "    \"IntSettingCache\": 12345," << std::endl;
			ss << "    \"StrSettingCache\": \"abab\"," << std::endl;
			ss << "    \"BoolSettingCache\": false," << std::endl;
			ss << "    \"IntSettingNoCache\": 56789," << std::endl;
			ss << "    \"StrSettingNoCache\": \"cdcd\"," << std::endl;
			ss << "    \"BoolSettingNoCache\": true," << std::endl;
			ss << "    \"Section\": " << std::endl;
			ss << "    {" << std::endl;
			ss << "        \"IntSettingCache\": 43210," << std::endl;
			ss << "        \"StrSettingCache\": \"baba\"," << std::endl;
			ss << "        \"BoolSettingCache\": false," << std::endl;
			ss << "        \"Subsection\": " << std::endl;
			ss << "        {" << std::endl;
			ss << "            \"IntSettingCache\": 87654," << std::endl;
			ss << "            \"StrSettingCache\": \"dcdc\"," << std::endl;
			ss << "            \"BoolSettingCache\": true" << std::endl;
			ss << "        }" << std::endl;
			ss << "    }" << std::endl;
			ss << "}";

			boost::filesystem::path settingsFilepath = folderPath / "MySettingsFile.json";
			boost::filesystem::remove(settingsFilepath);

			std::ofstream fileStream;
			fileStream.open(settingsFilepath.string());
			fileStream << ss.str();
			fileStream.close();
		}

	};

	TEST_F(SettingsServiceCacheTest, testGetSettingsReturnsValuesFromFile)
	{
		EXPECT_EQ( 56789, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ("cdcd", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(  true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

}}}

