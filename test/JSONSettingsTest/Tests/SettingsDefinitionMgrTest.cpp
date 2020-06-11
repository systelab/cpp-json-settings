#include "stdafx.h"
#include "TestSettingsDefinition.h"

#include "JSONSettings/Model/SettingDefinitionMgr.h"


namespace systelab { namespace setting { namespace unit_test {

	class SettingsDefinitionMgrTest : public ::testing::Test
	{
	};


	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerContainsExpectedFiles)
	{
		SettingDefinitionMgr::get().hasFile(MySettingsFile::FILENAME);
		SettingDefinitionMgr::get().hasFile(SecondSettingsFile::FILENAME);
		SettingDefinitionMgr::get().hasFile(EncryptedSettingsFile::FILENAME);
		SettingDefinitionMgr::get().hasFile(InvalidSettingsFile::FILENAME);
	}


	// Second settings file
	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerContainsExpectedSettingsForSecondSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(SecondSettingsFile::FILENAME);
		ASSERT_EQ(4, settings.size());

		EXPECT_TRUE(settings.find(SecondSettingsFile::RootIntSettingCache::PATH) != settings.end());
		EXPECT_TRUE(settings.find(SecondSettingsFile::RootDblSettingCache::PATH) != settings.end());
		EXPECT_TRUE(settings.find(SecondSettingsFile::RootStrSettingCache::PATH) != settings.end());
		EXPECT_TRUE(settings.find(SecondSettingsFile::RootBoolSettingCache::PATH) != settings.end());
	}

	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerSetsIncrementalIdentifiersForSettingsOfSecondSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(SecondSettingsFile::FILENAME);
		EXPECT_EQ(1, settings.find(SecondSettingsFile::RootIntSettingCache::PATH)->second.id);
		EXPECT_EQ(2, settings.find(SecondSettingsFile::RootDblSettingCache::PATH)->second.id);
		EXPECT_EQ(3, settings.find(SecondSettingsFile::RootStrSettingCache::PATH)->second.id);
		EXPECT_EQ(4, settings.find(SecondSettingsFile::RootBoolSettingCache::PATH)->second.id);
	}

	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerSetsDefinedDefaultValueForSettingsOfSecondSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(SecondSettingsFile::FILENAME);
		EXPECT_EQ(SettingValue(2222), settings.find(SecondSettingsFile::RootIntSettingCache::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(2.222), settings.find(SecondSettingsFile::RootDblSettingCache::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(std::string("SECOND")), settings.find(SecondSettingsFile::RootStrSettingCache::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(false), settings.find(SecondSettingsFile::RootBoolSettingCache::PATH)->second.defaultValue);
	}


	// Encrypted settings file
	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerContainsExpectedSettingsForEncryptedSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(EncryptedSettingsFile::FILENAME);
		ASSERT_EQ(4, settings.size());

		EXPECT_TRUE(settings.find(EncryptedSettingsFile::SectionIntSetting::PATH) != settings.end());
		EXPECT_TRUE(settings.find(EncryptedSettingsFile::SectionDblSetting::PATH) != settings.end());
		EXPECT_TRUE(settings.find(EncryptedSettingsFile::SectionStrSetting::PATH) != settings.end());
		EXPECT_TRUE(settings.find(EncryptedSettingsFile::SectionBoolSetting::PATH) != settings.end());
	}

	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerSetsIncrementalIdentifiersForSettingsOfEncryptedSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(EncryptedSettingsFile::FILENAME);
		EXPECT_EQ(1, settings.find(EncryptedSettingsFile::SectionIntSetting::PATH)->second.id);
		EXPECT_EQ(2, settings.find(EncryptedSettingsFile::SectionDblSetting::PATH)->second.id);
		EXPECT_EQ(3, settings.find(EncryptedSettingsFile::SectionStrSetting::PATH)->second.id);
		EXPECT_EQ(4, settings.find(EncryptedSettingsFile::SectionBoolSetting::PATH)->second.id);
	}

	TEST_F(SettingsDefinitionMgrTest, testSettingsDefinitionManagerSetsDefinedDefaultValueForSettingsOfEncryptedSettingsFile)
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(EncryptedSettingsFile::FILENAME);
		EXPECT_EQ(SettingValue(9999), settings.find(EncryptedSettingsFile::SectionIntSetting::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(9.9), settings.find(EncryptedSettingsFile::SectionDblSetting::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(std::string("99")), settings.find(EncryptedSettingsFile::SectionStrSetting::PATH)->second.defaultValue);
		EXPECT_EQ(SettingValue(true), settings.find(EncryptedSettingsFile::SectionBoolSetting::PATH)->second.defaultValue);
	}

}}}

