#include "stdafx.h"
#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"
#include "RapidJSONAdapter/JSONAdapter.h"

#include <boost/filesystem.hpp>


using namespace testing;

namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceNoCacheTest : public ::testing::Test
	{
	public:
		void SetUp()
		{
			auto folderPath = boost::filesystem::path("SettingsServiceNoCacheTest");
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			SET_JSON_SETTINGS_FOLDER(boost::filesystem::absolute(folderPath).string());
			m_settingsFilepath = folderPath / "MySettingsFile.json";

			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}
		}

		void writeSettingsFile(int intValue, const std::string& strValue, bool boolValue)
		{
			std::stringstream ss;
			ss << "{" << std::endl;
			ss << "    \"IntSettingNoCache\": " << intValue << "," << std::endl;
			ss << "    \"StrSettingNoCache\": \"" << strValue << "\"," << std::endl;
			ss << "    \"BoolSettingNoCache\": " << boolValue << std::endl;
			ss << "}";

			boost::filesystem::remove(m_settingsFilepath);

			std::ofstream fileStream;
			fileStream.open(m_settingsFilepath.string());
			fileStream << ss.str();
			fileStream.close();
		}

		std::string readSettingsFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_settingsFilepath.string());
			if (ifs)
			{
				std::stringstream ss;
				ss << ifs.rdbuf();
				fileContents = ss.str();
				ifs.close();
			}

			return fileContents;
		}

	protected:
		boost::filesystem::path m_settingsFilepath;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
	};

	TEST_F(SettingsServiceNoCacheTest, testGetSettingsReturnsValuesFromFile)
	{
		writeSettingsFile(123456, "file", true);

		EXPECT_EQ(123456, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ("file", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(  true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

	TEST_F(SettingsServiceNoCacheTest, testGetSettingsReturnsValuesFromFileWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingNoCache);
		GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingNoCache);
		GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache);

		writeSettingsFile(1111, "AAAA", false);

		EXPECT_EQ(  1111, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ("AAAA", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ( false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingIntWritesValueIntoFile)
	{
		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache, 7777);
		std::string expectedJSON = "{ \"IntSettingNoCache\": 7777 }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingStrWritesValueIntoFile)
	{
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingNoCache, "NewValue");
		std::string expectedJSON = "{ \"StrSettingNoCache\": \"NewValue\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingBoolWritesValueIntoFile)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache, true);
		std::string expectedJSON = "{ \"BoolSettingNoCache\": true }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


}}}

