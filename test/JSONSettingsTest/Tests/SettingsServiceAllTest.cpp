#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceAllTest : public SettingsServiceBaseTest
	{
	public:
		void SetUp()
		{
			SettingsServiceBaseTest::SetUp();
			setUpSettingsFile();
		}

		void TearDown()
		{
			SettingsServiceBaseTest::TearDown();
			SettingsService().clearCache();
		}

		void setUpSettingsFile()
		{
			std::stringstream ss;
			ss << "{" << std::endl;
			ss << "    \"IntSettingCache\": \"12345\"," << std::endl;
			ss << "    \"StrSettingCache\": \"abab\"," << std::endl;
			ss << "    \"DblSettingCache\": \"1.234\"," << std::endl;
			ss << "    \"BoolSettingCache\": \"false\"," << std::endl;
			ss << "    \"IntSettingNoCache\": \"56789\"," << std::endl;
			ss << "    \"StrSettingNoCache\": \"cdcd\"," << std::endl;
			ss << "    \"BoolSettingNoCache\": \"true\"," << std::endl;
			ss << "    \"Section\": " << std::endl;
			ss << "    {" << std::endl;
			ss << "        \"IntSettingCache\": \"43210\"," << std::endl;
			ss << "        \"DblSettingCache\": \"4.321\"," << std::endl;
			ss << "        \"StrSettingCache\": \"baba\"," << std::endl;
			ss << "        \"BoolSettingCache\": \"false\"," << std::endl;
			ss << "        \"Subsection\": " << std::endl;
			ss << "        {" << std::endl;
			ss << "            \"IntSettingCache\": \"87654\"," << std::endl;
			ss << "            \"DblSettingCache\": \"8.7654\"," << std::endl;
			ss << "            \"StrSettingCache\": \"dcdc\"," << std::endl;
			ss << "            \"BoolSettingCache\": \"true\"" << std::endl;
			ss << "        }" << std::endl;
			ss << "    }" << std::endl;
			ss << "}";

			std::string fileContents = ss.str();
			writeSettingsFile(fileContents);
		}
	};

	TEST_F(SettingsServiceAllTest, testGetSettingsReturnsValuesFromFile)
	{
		EXPECT_EQ( 12345, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache));
		EXPECT_EQ( 1.234, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingCache));
		EXPECT_EQ("abab", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache));
		EXPECT_EQ( false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));

		EXPECT_EQ( 56789, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ( 5.678, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache));
		EXPECT_EQ("cdcd", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(  true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));

		EXPECT_EQ( 43210, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ( 4.321, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting));
		EXPECT_EQ("baba", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ( false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));

		EXPECT_EQ( 87654, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SubsectionIntSetting));
		EXPECT_EQ(8.7654, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SubsectionDblSetting));
		EXPECT_EQ("dcdc", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SubsectionStrSetting));
		EXPECT_EQ(  true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SubsectionBoolSetting));
	}

	TEST_F(SettingsServiceAllTest, testSetSettingsUpdatesValuesIntoFile)
	{
		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache, 11111);
		SET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingCache, 1.1111);
		SET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache, "ZZZ");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);

		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache, 22222);
		SET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache, 2.2222);
		SET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache, "YYY");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache, false);

		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting, 33333);
		SET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting, 3.3333);
		SET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting, "WWW");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting, true);

		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SubsectionIntSetting, 44444);
		SET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SubsectionDblSetting, 4.4444);
		SET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SubsectionStrSetting, "XXX");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SubsectionBoolSetting, false);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"11111\"," << std::endl;
		ss << "    \"DblSettingCache\": \"1.1111\"," << std::endl;
		ss << "    \"StrSettingCache\": \"ZZZ\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"true\"," << std::endl;
		ss << "    \"IntSettingNoCache\": \"22222\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"2.2222\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"YYY\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"false\"," << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"33333\"," << std::endl;
		ss << "        \"DblSettingCache\": \"3.3333\"," << std::endl;
		ss << "        \"StrSettingCache\": \"WWW\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"true\"," << std::endl;
		ss << "        \"Subsection\": " << std::endl;
		ss << "        {" << std::endl;
		ss << "            \"IntSettingCache\": \"44444\"," << std::endl;
		ss << "            \"DblSettingCache\": \"4.4444\"," << std::endl;
		ss << "            \"StrSettingCache\": \"XXX\"," << std::endl;
		ss << "            \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "        }" << std::endl;
		ss << "    }" << std::endl;
		ss << "}";

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

}}}

