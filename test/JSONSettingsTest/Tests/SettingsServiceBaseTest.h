#pragma once

#include "JSONSettings/Services/SettingsService.h"
#include "JSONSettings/Services/SettingsMacros.h"

#include "TestSettingsDefinition.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"
#include "RapidJSONAdapter/JSONAdapter.h"

#include <boost/filesystem.hpp>


using namespace testing;

namespace systelab { namespace setting { namespace unit_test {

	class SettingsServiceBaseTest : public ::testing::Test
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
			m_settingsFilepath = boost::filesystem::absolute(folderPath) / "MySettingsFile.json";

			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_settingsFilepath))
			{
				boost::filesystem::remove(m_settingsFilepath);
			}
		}

		void writeSettingsFile(const std::string& fileContents)
		{
			boost::filesystem::remove(m_settingsFilepath);

			std::ofstream fileStream;
			fileStream.open(m_settingsFilepath.string());
			fileStream << fileContents;
			fileStream.close();
		}

		std::string readSettingsFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_settingsFilepath.string());
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
		boost::filesystem::path m_settingsFilepath;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
	};

}}}

