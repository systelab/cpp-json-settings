#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceNoCacheTest : public SettingsServiceBaseTest
	{
	public:
		void SetUp()
		{
			SettingsServiceBaseTest::SetUp();
		}

		void TearDown()
		{
			SettingsServiceBaseTest::TearDown();
		}

		void writeNoCacheSettingsIntoFile(int intValue, double dblValue, const std::string& strValue, bool boolValue)
		{
			std::stringstream ss;
			ss.precision(std::numeric_limits<double>::max_digits10 - 1);
			ss << "{" << std::endl;
			ss << "    \"IntSettingNoCache\": \"" << intValue << "\"," << std::endl;
			ss << "    \"DblSettingNoCache\": \"" << dblValue << "\"," << std::endl;
			ss << "    \"StrSettingNoCache\": \"" << strValue << "\"," << std::endl;
			ss << "    \"BoolSettingNoCache\": \"" << (boolValue ? "true" : "false") << "\"" << std::endl;
			ss << "}";

			std::string fileContents = ss.str();
			writeSettingsFile(fileContents);
		}
	};


	// GetSettings
	TEST_F(SettingsServiceNoCacheTest, testGetSettingsReturnsDefaultValuesWhenNoFile)
	{
		EXPECT_EQ( 5678, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ(5.678, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache));
		EXPECT_EQ( "cd", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

	TEST_F(SettingsServiceNoCacheTest, testGetSettingsReturnsValuesFromFile)
	{
		writeNoCacheSettingsIntoFile(123456, 123456.789012, "file", true);

		EXPECT_EQ(123456,        GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ(123456.789012, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache));
		EXPECT_EQ("file",        GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(true,          GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}

	TEST_F(SettingsServiceNoCacheTest, testGetSettingsReturnsValuesFromFileWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache);
		GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache);
		GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache);
		GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache);

		writeNoCacheSettingsIntoFile(1111, 222.333, "AAAA", false);

		EXPECT_EQ(   1111, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache));
		EXPECT_EQ(222.333, GET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache));
		EXPECT_EQ( "AAAA", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache));
		EXPECT_EQ(  false, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache));
	}


	// SetSettings - Not existing file
	TEST_F(SettingsServiceNoCacheTest, testSetSettingIntWritesValueIntoFile)
	{
		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache, 7777);
		std::string expectedJSON = "{ \"IntSettingNoCache\": \"7777\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingDblWritesValueIntoFile)
	{
		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, RootDblSettingNoCache, 777.888);
		std::string expectedJSON = "{ \"DblSettingNoCache\": \"777.888\" }";
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
		std::string expectedJSON = "{ \"BoolSettingNoCache\": \"true\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingsWritesAllValuesIntoFile)
	{
		SET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingNoCache, 7777);
		SET_JSON_SETTING_DBL (SettingsService(), MySettingsFile, RootDblSettingNoCache, 777.888);
		SET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingNoCache, "NewValue");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingNoCache\": \"7777\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"777.888\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"NewValue\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"true\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Existing file
	TEST_F(SettingsServiceNoCacheTest, testSetSettingIntUpdatesValueFromExistingFile)
	{
		writeNoCacheSettingsIntoFile(1111, 777.888, "AAAA", false);

		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingNoCache, 2222);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingNoCache\": \"2222\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"777.888\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"AAAA\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"false\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingDblUpdatesValueFromExistingFile)
	{
		writeNoCacheSettingsIntoFile(1111, 777.888, "AAAA", false);

		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, RootDblSettingNoCache, 999.444);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingNoCache\": \"1111\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"999.444\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"AAAA\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"false\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingStrUpdatesValueFromExistingFile)
	{
		writeNoCacheSettingsIntoFile(1111, 777.888, "AAAA", false);

		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingNoCache, "BBBBBB");

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingNoCache\": \"1111\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"777.888\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"BBBBBB\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"false\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceNoCacheTest, testSetSettingBoolUpdatesValueFromExistingFile)
	{
		writeNoCacheSettingsIntoFile(1111, 777.888, "AAAA", false);

		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingNoCache, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingNoCache\": \"1111\"," << std::endl;
		ss << "    \"DblSettingNoCache\": \"777.888\"," << std::endl;
		ss << "    \"StrSettingNoCache\": \"AAAA\"," << std::endl;
		ss << "    \"BoolSettingNoCache\": \"true\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

}}}

