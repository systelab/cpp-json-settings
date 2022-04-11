#include "stdafx.h"
#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"
#include "RapidJSONAdapter/JSONAdapter.h"
#include "CaeserCypherEncryptionAdapter/EncryptionAdapter.h"

#include <boost/filesystem.hpp>


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceEncryptionTest : public ::testing::Test
	{
	public:
		void SetUp()
		{
			auto folderPath = boost::filesystem::path("SettingsServiceTest");
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			SET_JSON_SETTINGS_FOLDER(boost::filesystem::absolute(folderPath).string());
			m_settingsFilepath = boost::filesystem::absolute(folderPath) / "EncryptedSettingsFile.json";

			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}

			m_encryptionKey = "Key";
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}
			SettingsService(m_encryptionAdapter).clearCache();
		}

		void writeSectionSettingsIntoEncryptedFile(int intValue, double dblValue, const std::string& strValue, bool boolValue)
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
			writeEncryptedSettingsFile(fileContents);
		}

		void writeEncryptedSettingsFile(const std::string& fileContents)
		{
			boost::filesystem::remove(m_settingsFilepath);

			std::ofstream fileStream;
			fileStream.open(m_settingsFilepath.string(), std::ofstream::binary);
			fileStream << m_encryptionAdapter.encryptString([this]() { return m_encryptionKey; }, fileContents);
			fileStream.close();
		}

		std::string readDecryptedSettingsFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_settingsFilepath.string(), std::ios::binary);
			if (ifs)
			{
				std::stringstream ss;
				ss << ifs.rdbuf();
				fileContents = m_encryptionAdapter.decryptString([this]() { return m_encryptionKey; }, ss.str());
				ifs.close();
			}

			return fileContents;
		}

	protected:
		boost::filesystem::path m_settingsFilepath;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
		systelab::encryption::caeser_cypher::EncryptionAdapter m_encryptionAdapter;
		std::string m_encryptionKey;
	};


	// GetSettings
	TEST_F(SettingsServiceEncryptionTest, testGetSettingsReturnsDefaultValuesWhenNoFileNorCache)
	{
		EXPECT_EQ( 9999, GET_JSON_SETTING_INT (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting));
		EXPECT_EQ(  9.9, GET_JSON_SETTING_DBL (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting));
		EXPECT_EQ( "99", GET_JSON_SETTING_STR (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceEncryptionTest, testGetSettingsReturnsValuesFromFileWhenNotInCache)
	{
		writeSectionSettingsIntoEncryptedFile(123456, 1.2345, "file", false);

		EXPECT_EQ(123456, GET_JSON_SETTING_INT (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting));
		EXPECT_EQ(1.2345, GET_JSON_SETTING_DBL (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting));
		EXPECT_EQ("file", GET_JSON_SETTING_STR (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting));
		EXPECT_EQ(false,  GET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceEncryptionTest, testGetSettingsReturnsValuesFromCacheWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting);
		GET_JSON_SETTING_DBL (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting);
		GET_JSON_SETTING_STR (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting);
		GET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting);

		writeSectionSettingsIntoEncryptedFile(1111, 1.2345, "AAAA", false);

		EXPECT_EQ( 9999, GET_JSON_SETTING_INT (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting));
		EXPECT_EQ(  9.9, GET_JSON_SETTING_DBL (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting));
		EXPECT_EQ( "99", GET_JSON_SETTING_STR (SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting));
	}


	// SetSettings - Not existing file
	TEST_F(SettingsServiceEncryptionTest, testSetSettingIntWritesValueIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting, 7777);
		std::string expectedJSON = "{ \"Section\": { \"IntSettingCache\": \"7777\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingDblWritesValueIntoFile)
	{
		SET_JSON_SETTING_DBL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting, 77.77);
		std::string expectedJSON = "{ \"Section\": { \"DblSettingCache\": \"77.77\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingStrWritesValueIntoFile)
	{
		SET_JSON_SETTING_STR(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting, "NewValue");
		std::string expectedJSON = "{ \"Section\": { \"StrSettingCache\": \"NewValue\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingBoolWritesValueIntoFile)
	{
		SET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting, true);
		std::string expectedJSON = "{ \"Section\": { \"BoolSettingCache\": \"true\" } }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingsWritesAllValuesIntoFile)
	{
		SET_JSON_SETTING_INT(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting, 7777);
		SET_JSON_SETTING_DBL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting, 77.77);
		SET_JSON_SETTING_STR(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting, "NewValue");
		SET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting, true);

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
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Existing file
	TEST_F(SettingsServiceEncryptionTest, testSetSettingIntUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoEncryptedFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_INT(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting, 2222);

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
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingDblUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoEncryptedFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_DBL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting, 22.22);

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
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingStrUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoEncryptedFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_STR(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting, "BBBBBB");

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
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingBoolUpdatesValueFromExistingFile)
	{
		writeSectionSettingsIntoEncryptedFile(1111, 11.11, "AAAA", false);

		SET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting, true);

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
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readDecryptedSettingsFile(), m_jsonAdapter));
	}


	// SetSettings - Cache updated after set
	TEST_F(SettingsServiceEncryptionTest, testSetSettingIntUpdatesCache)
	{
		SET_JSON_SETTING_INT(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting, 7777);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(7777, GET_JSON_SETTING_INT(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionIntSetting));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingDblUpdatesCache)
	{
		SET_JSON_SETTING_DBL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting, 77.77);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(77.77, GET_JSON_SETTING_DBL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionDblSetting));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingStrUpdatesCache)
	{
		SET_JSON_SETTING_STR(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting, "NewValue");
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ("NewValue", GET_JSON_SETTING_STR(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionStrSetting));
	}

	TEST_F(SettingsServiceEncryptionTest, testSetSettingBoolUpdatesCache)
	{
		SET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting, true);
		boost::filesystem::remove(m_settingsFilepath);
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(m_encryptionAdapter), EncryptedSettingsFile, SectionBoolSetting));
	}

}}}

