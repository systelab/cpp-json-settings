#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceErrorTest : public SettingsServiceBaseTest
	{
	public:
		void SetUp()
		{
			SettingsServiceBaseTest::SetUp();
		}

		void TearDown()
		{
			SettingsServiceBaseTest::TearDown();
			SettingsService().clearCache();
		}
	};


	// File error
	TEST_F(SettingsServiceErrorTest, testGetSettingsReturnsDefaultValueWhenFileContentsNotAJSON)
	{
		writeSettingsFile("Settings file is not a JSON");

		EXPECT_EQ(4321, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ("ba", GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingIntOverwritesNoJSONFileContents)
	{
		writeSettingsFile("Settings file is not a JSON");

		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 123456789);

		std::string expectedJSON = "{ \"IntSettingCache\": \"123456789\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingStrOverwritesNoJSONFileContents)
	{
		writeSettingsFile("Settings file is not a JSON");

		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache, "ERROR");

		std::string expectedJSON = "{ \"StrSettingCache\": \"ERROR\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingBoolOverwritesNoJSONFileContents)
	{
		writeSettingsFile("Settings file is not a JSON");

		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);

		std::string expectedJSON = "{ \"BoolSettingCache\": \"true\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


}}}

