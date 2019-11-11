#include "stdafx.h"
#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"


using namespace testing;

namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceDefaultTest : public ::testing::Test
	{
		void SetUp()
		{
		}
	};


	TEST_F(SettingsServiceDefaultTest, testGetRootCacheSettingsReturnsDefaultValues)
	{
		EXPECT_EQ(1234, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache));
		EXPECT_EQ("ab", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));
	}

	TEST_F(SettingsServiceDefaultTest, testGetRootNoCacheSettingsReturnsDefaultValues)
	{
		EXPECT_EQ( 5678, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ( "cd", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

	TEST_F(SettingsServiceDefaultTest, testGetSectionSettingsReturnsDefaultValues)
	{
		EXPECT_EQ( 4321, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ( "ba", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceDefaultTest, testGetSubsectionSettingsReturnsDefaultValues)
	{
		EXPECT_EQ( 8765, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SubsectionIntSetting));
		EXPECT_EQ( "dc", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SubsectionStrSetting));
		EXPECT_EQ(false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SubsectionBoolSetting));
	}

}}}

