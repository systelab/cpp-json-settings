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

	TEST_F(SettingsServiceErrorTest, testGetSettingsReturnsDefaultValueWhenFileContentsNotAJSON)
	{
		writeSettingsFile("Settings file is not a JSON");

		EXPECT_EQ(4321, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ("ba", GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

}}}

