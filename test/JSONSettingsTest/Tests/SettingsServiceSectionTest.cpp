#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceSectionTest : public SettingsServiceBaseTest
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

		void writeSectionSettingsIntoFile(int intValue, double dblValue, const std::string& strValue, bool boolValue)
		{
			std::stringstream ss;
			ss.precision(std::numeric_limits<double>::max_digits10 - 1);
			ss << "{" << std::endl;
			ss << "    \"Section\": " << std::endl;
			ss << "    {" << std::endl;
			ss << "        \"IntSettingCache\": \"" << intValue << "\"," << std::endl;
			ss << "        \"DblSettingCache\": \"" << dblValue << "\"," << std::endl;
			ss << "        \"StrSettingCache\": \"" << strValue << "\"," << std::endl;
			ss << "        \"BoolSettingCache\": \"" << (boolValue ? "true" : "false") << "\"" << std::endl;
			ss << "    }" << std::endl;
			ss << "}";

			std::string fileContents = ss.str();
			writeSettingsFile(fileContents);
		}
	};


	// GetSettings
	TEST_F(SettingsServiceSectionTest, testGetSettingsReturnsDefaultValuesWhenNoFileNorCache)
	{
		EXPECT_EQ( 4321, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ(4.321, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting));
		EXPECT_EQ( "ba", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceSectionTest, testGetSettingsReturnsValuesFromFileWhenNotInCache)
	{
		writeSectionSettingsIntoFile(123456, 1.2345, "file", false);

		EXPECT_EQ(123456, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ(1.2345, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting));
		EXPECT_EQ("file", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ(false,  GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceSectionTest, testGetSettingsReturnsValuesFromCacheWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting);
		GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting);
		GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting);
		GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting);

		writeSectionSettingsIntoFile(1111, 1.2345, "AAAA", false);

		EXPECT_EQ( 4321, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ(4.321, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, SectionDblSetting));
		EXPECT_EQ( "ba", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}


	// SetSettings - Not existing file
	TEST_F(SettingsServiceSectionTest, testSetSettingIntWritesValueIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting, 7777);
		std::string expectedJSON = "{ \"Section\": { \"IntSettingCache\": \"7777\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingDblWritesValueIntoFile)
	{
		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting, 77.77);
		std::string expectedJSON = "{ \"Section\": { \"DblSettingCache\": \"77.77\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingStrWritesValueIntoFile)
	{
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting, "NewValue");
		std::string expectedJSON = "{ \"Section\": { \"StrSettingCache\": \"NewValue\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingBoolWritesValueIntoFile)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting, true);
		std::string expectedJSON = "{ \"Section\": { \"BoolSettingCache\": \"true\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingsWritesAllValuesIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting, 7777);
		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting, 77.77);
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting, "NewValue");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"7777\"," << std::endl;
		ss << "        \"DblSettingCache\": \"77.77\"," << std::endl;
		ss << "        \"StrSettingCache\": \"NewValue\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "    }" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Existing file
	TEST_F(SettingsServiceSectionTest, testSetSettingIntUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting, 2222);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"2222\"," << std::endl;
		ss << "        \"DblSettingCache\": \"11.11\"," << std::endl;
		ss << "        \"StrSettingCache\": \"AAAA\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "    }" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingDblUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting, 22.22);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"1111\"," << std::endl;
		ss << "        \"DblSettingCache\": \"22.22\"," << std::endl;
		ss << "        \"StrSettingCache\": \"AAAA\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "    }" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingStrUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting, "BBBBBB");

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"1111\"," << std::endl;
		ss << "        \"DblSettingCache\": \"11.11\"," << std::endl;
		ss << "        \"StrSettingCache\": \"BBBBBB\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "    }" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingBoolUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"Section\": " << std::endl;
		ss << "    {" << std::endl;
		ss << "        \"IntSettingCache\": \"1111\"," << std::endl;
		ss << "        \"DblSettingCache\": \"11.11\"," << std::endl;
		ss << "        \"StrSettingCache\": \"AAAA\"," << std::endl;
		ss << "        \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "    }" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Cache updated after set
	TEST_F(SettingsServiceSectionTest, testSetSettingIntUpdatesCache)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting, 7777);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(7777, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingDblUpdatesCache)
	{
		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting, 77.77);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(77.77, GET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingStrUpdatesCache)
	{
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting, "NewValue");
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ("NewValue", GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting));
	}

	TEST_F(SettingsServiceSectionTest, testSetSettingBoolUpdatesCache)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting, true);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

}}}

