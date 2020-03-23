#include "stdafx.h"
#include "SettingsServiceBaseTest.h"

#include "JSONSettings/Model/SettingsCache.h"


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

		EXPECT_EQ( 4321, GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionIntSetting));
		EXPECT_EQ(4.321, GET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionDblSetting));
		EXPECT_EQ( "ba", GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionStrSetting));
		EXPECT_EQ( true, GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionBoolSetting));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingIntOverwritesNoJSONFileContents)
	{
		writeSettingsFile("Settings file is not a JSON");

		SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, RootIntSettingCache, 123456789);

		std::string expectedJSON = "{ \"IntSettingCache\": \"123456789\" }";
		EXPECT_TRUE(json::test_utility::compareJSONs(expectedJSON, readSettingsFile(), m_jsonAdapter));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingDblOverwritesNoJSONFileContents)
	{
		writeSettingsFile("Settings file is not a JSON");

		SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, RootDblSettingCache, 1.2345678);

		std::string expectedJSON = "{ \"DblSettingCache\": \"1.2345678\" }";
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


	// Wrong type
	TEST_F(SettingsServiceErrorTest, testGetSettingIntThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionDblSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionStrSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionBoolSetting), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testGetSettingDblThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(GET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionIntSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionStrSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionBoolSetting), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testGetSettingStrThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionIntSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionDblSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionBoolSetting), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testGetSettingBoolThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionIntSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionDblSetting), std::runtime_error);
		ASSERT_THROW(GET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionStrSetting), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingIntThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionDblSetting, 1), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionStrSetting, 1), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_INT(SettingsService(), MySettingsFile, SectionBoolSetting, 1), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingDblThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionIntSetting, 1.1), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionStrSetting, 1.1), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_DBL(SettingsService(), MySettingsFile, SectionBoolSetting, 1.1), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingStrThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionIntSetting, "A"), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionDblSetting, "A"), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_STR(SettingsService(), MySettingsFile, SectionBoolSetting, "A"), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingBoolThrowsExceptionWhenNotForDefinedType)
	{
		ASSERT_THROW(SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionIntSetting, true), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionDblSetting, true), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_BOOL(SettingsService(), MySettingsFile, SectionStrSetting, true), std::runtime_error);
	}


	// Undefined setting/file
	TEST_F(SettingsServiceErrorTest, testGetSettingThrowsExceptionWhenGivenSettingNotDefined)
	{
		ASSERT_THROW(SettingsService().getSettingInteger("MySettingsFile.json", "NotExistingSettingInt"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingDouble("MySettingsFile.json", "NotExistingSettingDbl"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingString("MySettingsFile.json", "NotExistingSettingStr"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingBoolean("MySettingsFile.json", "NotExistingSettingBool"), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testGetSettingThrowsExceptionWhenGivenFileNotDefined)
	{
		ASSERT_THROW(SettingsService().getSettingInteger("NotDefinedSettingsFile.json", "SettingInt"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingDouble("NotDefinedSettingsFile.json", "SettingDbl"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingString("NotDefinedSettingsFile.json", "SettingStr"), std::runtime_error);
		ASSERT_THROW(SettingsService().getSettingBoolean("NotDefinedSettingsFile.json", "SettingBool"), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingThrowsExceptionWhenGivenSettingNotDefined)
	{
		ASSERT_THROW(SettingsService().setSettingInteger("MySettingsFile.json", "NotExistingSettingInt", 1), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingDouble("MySettingsFile.json", "NotExistingSettingDbl", 1.1), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingString("MySettingsFile.json", "NotExistingSettingStr", "A"), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingBoolean("MySettingsFile.json", "NotExistingSettingBool", true), std::runtime_error);
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingThrowsExceptionWhenGivenFileNotDefined)
	{
		ASSERT_THROW(SettingsService().setSettingInteger("NotDefinedSettingsFile.json", "SettingInt", 1), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingDouble("NotDefinedSettingsFile.json", "SettingDbl", 1.1), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingString("NotDefinedSettingsFile.json", "SettingStr", "A"), std::runtime_error);
		ASSERT_THROW(SettingsService().setSettingBoolean("NotDefinedSettingsFile.json", "SettingBool", true), std::runtime_error);
	}


	// Invalid file
	TEST_F(SettingsServiceErrorTest, testGetSettingReturnsDefaultValueWhenFileIsInvalid)
	{
		EXPECT_EQ(111,  GET_JSON_SETTING_INT (SettingsService(), InvalidSettingsFile, IntSetting));
		EXPECT_EQ(1.1,  GET_JSON_SETTING_DBL (SettingsService(), InvalidSettingsFile, DblSetting));
		EXPECT_EQ("A",  GET_JSON_SETTING_STR (SettingsService(), InvalidSettingsFile, StrSetting));
		EXPECT_EQ(true, GET_JSON_SETTING_BOOL(SettingsService(), InvalidSettingsFile, BoolSetting));
	}

	TEST_F(SettingsServiceErrorTest, testSetSettingThrowsExceptionWhenFileIsInvalid)
	{
		ASSERT_THROW(SET_JSON_SETTING_INT(SettingsService(), InvalidSettingsFile, IntSetting, 222), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_DBL(SettingsService(), InvalidSettingsFile, DblSetting, 2.2), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_STR(SettingsService(), InvalidSettingsFile, StrSetting, "B"), std::runtime_error);
		ASSERT_THROW(SET_JSON_SETTING_BOOL(SettingsService(), InvalidSettingsFile, BoolSetting, false), std::runtime_error);
	}


	// Not in cache
	TEST_F(SettingsServiceErrorTest, testGetSettingFromCacheThrowsExceptionWhenNotFound)
	{
		ASSERT_THROW(SettingsCache().getSetting<int>("MySettingsFile.json", "SettingNotInCache"), std::runtime_error);
	}

}}}

