#pragma once

#include "TestSettingsDefinition.h"

#include "CaeserCypherEncryptionAdapter/EncryptionAdapter.h"
#include "RapidJSONAdapter/JSONAdapter.h"

#include <boost/filesystem.hpp>


using namespace testing;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsEndpointBaseTest : public Test
	{
	public:
		void SetUp()
		{
			m_settingsFolderPath = boost::filesystem::path("SettingsEndpointTest");
			if (!boost::filesystem::exists(m_settingsFolderPath))
			{
				boost::filesystem::create_directories(m_settingsFolderPath);
			}

			SET_JSON_SETTINGS_FOLDER(boost::filesystem::absolute(m_settingsFolderPath).string());
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_settingsFolderPath))
			{
				boost::filesystem::remove(m_settingsFolderPath);
			}
		}

		void removeSettingsFile(const SettingsFile& file)
		{
			auto settingsFilepath = boost::filesystem::absolute(m_settingsFolderPath) / file;
			if (boost::filesystem::exists(settingsFilepath))
			{
				boost::filesystem::remove(settingsFilepath);
			}
		}

		void writeSettingsFile(const SettingsFile& file, const std::string& contents)
		{
			removeSettingsFile(file);

			std::ofstream fileStream;
			auto settingsFilepath = boost::filesystem::absolute(m_settingsFolderPath) / file;
			fileStream.open(settingsFilepath.string());
			fileStream << contents;
			fileStream.close();
		}

		std::string readSettingsFile(const SettingsFile& file)
		{
			std::string fileContents;
			auto settingsFilepath = boost::filesystem::absolute(m_settingsFolderPath) / file;
			std::ifstream ifs(settingsFilepath.string());
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
		boost::filesystem::path m_settingsFolderPath;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
		systelab::encryption::caeser_cypher::EncryptionAdapter m_encryptionAdapter;
	};

}}}}

