#include "stdafx.h"
#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"


using namespace testing;

namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceTest : public ::testing::Test
	{
	};


	TEST_F(SettingsServiceTest, testGetSettingReturnsDefaultValue)
	{
		EXPECT_EQ(1234, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache));

		
	}

}}}

