#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceCacheTest : public SettingsServiceBaseTest
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

		void writeCacheSettingsIntoFile(int intValue, const std::string& strValue, bool boolValue)
		{
			std::stringstream ss;
			ss << "{" << std::endl;
			ss << "    \"IntSettingCache\": \"" << intValue << "\"," << std::endl;
			ss << "    \"StrSettingCache\": \"" << strValue << "\"," << std::endl;
			ss << "    \"BoolSettingCache\": \"" << (boolValue ? "true" : "false") << "\"" << std::endl;
			ss << "}";

			std::string fileContents = ss.str();
			writeSettingsFile(fileContents);
		}
	};


	// GetSettings
	TEST_F(SettingsServiceCacheTest, testGetSettingsReturnsDefaultValuesWhenNoFileNorCache)
	{
		EXPECT_EQ(1234, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache));
		EXPECT_EQ("ab", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));
	}

	TEST_F(SettingsServiceCacheTest, testGetSettingsReturnsValuesFromFileWhenNotInCache)
	{
		writeCacheSettingsIntoFile(123456, "file", false);

		EXPECT_EQ(123456, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache));
		EXPECT_EQ("file", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache));
		EXPECT_EQ(false,  GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));
	}

	TEST_F(SettingsServiceCacheTest, testGetSettingsReturnsValuesFromCacheWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache);
		GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache);
		GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache);

		writeCacheSettingsIntoFile(1111, "AAAA", false);

		EXPECT_EQ(1234, GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache));
		EXPECT_EQ("ab", GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));
	}


	// SetSettings - Not existing file
	TEST_F(SettingsServiceCacheTest, testSetSettingIntWritesValueIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 7777);
		std::string expectedJSON = "{ \"IntSettingCache\": \"7777\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingStrWritesValueIntoFile)
	{
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache, "NewValue");
		std::string expectedJSON = "{ \"StrSettingCache\": \"NewValue\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingBoolWritesValueIntoFile)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);
		std::string expectedJSON = "{ \"BoolSettingCache\": \"true\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingsWritesAllValuesIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 7777);
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache, "NewValue");
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"7777\"," << std::endl;
		ss << "    \"StrSettingCache\": \"NewValue\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Existing file
	TEST_F(SettingsServiceCacheTest, testSetSettingIntUpdatesValueFromExistingFile)
	{
		writeCacheSettingsIntoFile(1111, "AAAA", false);

		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 2222);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"2222\"," << std::endl;
		ss << "    \"StrSettingCache\": \"AAAA\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingStrUpdatesValueFromExistingFile)
	{
		writeCacheSettingsIntoFile(1111, "AAAA", false);

		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache, "BBBBBB");

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"1111\"," << std::endl;
		ss << "    \"StrSettingCache\": \"BBBBBB\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"false\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingBoolUpdatesValueFromExistingFile)
	{
		writeCacheSettingsIntoFile(1111, "AAAA", false);

		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"1111\"," << std::endl;
		ss << "    \"StrSettingCache\": \"AAAA\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "}" << std::endl;

		std::string expectedJSON = ss.str();
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Cache updated after set
	TEST_F(SettingsServiceCacheTest, testSetSettingIntUpdatesCache)
	{
		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 7777);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(7777, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingStrUpdatesCache)
	{
		SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache, "NewValue");
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ("NewValue", GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, RootStrSettingCache));
	}

	TEST_F(SettingsServiceCacheTest, testSetSettingBoolUpdatesCache)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache, true);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache));
	}

}}}

