#include "stdafx.h"
#include "SettingsServiceBaseTest.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceMultipleFilesTest : public SettingsServiceBaseTest
	{
	public:
		void SetUp()
		{
			SettingsServiceBaseTest::SetUp();
			setUpFirstSettingsFile();

			std::string folderPath = SettingDefinitionMgr::get().getSettingsFolderPath();
			m_settingsSecondFilepath = boost::filesystem::absolute(folderPath) / "SecondSettingsFile.json";

			if (boost::filesystem::exists(m_settingsSecondFilepath))
			{
				boost::filesystem::remove(m_settingsSecondFilepath);
			}
		}

		void TearDown()
		{
			SettingsServiceBaseTest::TearDown();
			SettingsService().clearCache();
		}

		void setUpFirstSettingsFile()
		{
			std::stringstream ss;
			ss << "{" << std::endl;
			ss << "    \"IntSettingCache\": \"123\"," << std::endl;
			ss << "    \"StrSettingCache\": \"NotThis\"," << std::endl;
			ss << "    \"BoolSettingCache\": \"true\"" << std::endl;
			ss << "}";

			m_firstSettingsFileContent = ss.str();
			writeSettingsFile(m_firstSettingsFileContent);
		}

		void writeSecondSettingsFile(const std::string& fileContents)
		{
			boost::filesystem::remove(m_settingsSecondFilepath);

			std::ofstream fileStream;
			fileStream.open(m_settingsSecondFilepath.string());
			fileStream << fileContents;
			fileStream.close();
		}

		std::string readSecondSettingsFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_settingsSecondFilepath.string());
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
		boost::filesystem::path m_settingsSecondFilepath;
		std::string m_firstSettingsFileContent;
	};


	// Get settings
	TEST_F(SettingsServiceMultipleFilesTest, testGetSettingsForSecondFileReturnsDefaultValues)
	{
		EXPECT_EQ(    2222, GET_JSON_SETTING_INT (SettingsService(), SecondSettingsFile, RootIntSettingCache));
		EXPECT_EQ("SECOND", GET_JSON_SETTING_STR (SettingsService(), SecondSettingsFile, RootStrSettingCache));
		EXPECT_EQ(   false, GET_JSON_SETTING_BOOL(SettingsService(), SecondSettingsFile, RootBoolSettingCache));
	}

	TEST_F(SettingsServiceMultipleFilesTest, testGetSettingsForSecondFileReturnsValuesFromFile)
	{
		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"3333\"," << std::endl;
		ss << "    \"StrSettingCache\": \"CCCC\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "}";
		writeSecondSettingsFile(ss.str());

		EXPECT_EQ(  3333, GET_JSON_SETTING_INT (SettingsService(), SecondSettingsFile, RootIntSettingCache));
		EXPECT_EQ("CCCC", GET_JSON_SETTING_STR (SettingsService(), SecondSettingsFile, RootStrSettingCache));
		EXPECT_EQ(  true, GET_JSON_SETTING_BOOL(SettingsService(), SecondSettingsFile, RootBoolSettingCache));
	}

	TEST_F(SettingsServiceMultipleFilesTest, testGetSettingsForSecondFileReturnsValuesFromCacheWhenAlreadyRead)
	{
		GET_JSON_SETTING_INT (SettingsService(), MySettingsFile, RootIntSettingCache);
		GET_JSON_SETTING_STR (SettingsService(), MySettingsFile, RootStrSettingCache);
		GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, RootBoolSettingCache);

		GET_JSON_SETTING_INT (SettingsService(), SecondSettingsFile, RootIntSettingCache);
		GET_JSON_SETTING_STR (SettingsService(), SecondSettingsFile, RootStrSettingCache);
		GET_JSON_SETTING_BOOL(SettingsService(), SecondSettingsFile, RootBoolSettingCache);

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"IntSettingCache\": \"3333\"," << std::endl;
		ss << "    \"StrSettingCache\": \"CCCC\"," << std::endl;
		ss << "    \"BoolSettingCache\": \"true\"" << std::endl;
		ss << "}";
		writeSecondSettingsFile(ss.str());

		EXPECT_EQ(    2222, GET_JSON_SETTING_INT (SettingsService(), SecondSettingsFile, RootIntSettingCache));
		EXPECT_EQ("SECOND", GET_JSON_SETTING_STR (SettingsService(), SecondSettingsFile, RootStrSettingCache));
		EXPECT_EQ(   false, GET_JSON_SETTING_BOOL(SettingsService(), SecondSettingsFile, RootBoolSettingCache));
	}

	
	// Set settings
	TEST_F(SettingsServiceMultipleFilesTest, testSetSettingIntForSecondFileDoesNotWriteOnFirstFile)
	{
		SET_JSON_SETTING_INT(SettingsService(), SecondSettingsFile, RootIntSettingCache, 7777);
		EXPECT_TRUE(json::test_utility::compareJSONs(m_firstSettingsFileContent, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceMultipleFilesTest, testSetSettingStrForSecondFileDoesNotWriteOnFirstFile)
	{
		SET_JSON_SETTING_STR(SettingsService(), SecondSettingsFile, RootStrSettingCache, "NewValue");
		EXPECT_TRUE(json::test_utility::compareJSONs(m_firstSettingsFileContent, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceMultipleFilesTest, testSetSettingBoolForSecondFileDoesNotWriteOnFirstFile)
	{
		SET_JSON_SETTING_BOOL(SettingsService(), SecondSettingsFile, RootBoolSettingCache, true);
		EXPECT_TRUE(json::test_utility::compareJSONs(m_firstSettingsFileContent, readSettingsFile(), m_jsonAdapter));
	}

}}}

