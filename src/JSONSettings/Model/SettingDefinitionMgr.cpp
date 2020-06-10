#include "stdafx.h"
#include "SettingDefinitionMgr.h"


namespace systelab { namespace setting {

	SettingDefinitionMgr::SettingDefinitionMgr()
		:m_settings()
		,m_settingsFileEncryptionKey()
		,m_settingsFolderPath("")
	{
	}

	SettingDefinitionMgr::~SettingDefinitionMgr() = default;

	SettingDefinitionMgr& SettingDefinitionMgr::get()
	{
		static SettingDefinitionMgr instance;
		return instance;
	}

	bool SettingDefinitionMgr::hasFile(const SettingsFile& file) const
	{
		return m_settings.find(file) != m_settings.end();
	}

	bool SettingDefinitionMgr::hasSetting(const SettingsFile& file,
										  const SettingPath& sectionPath) const
	{
		if (hasFile(file))
		{
			const auto& fileSettings =  m_settings.at(file);
			return fileSettings.find(sectionPath) != fileSettings.end();
		}

		return false;
	}

	const SettingDefinition& SettingDefinitionMgr::getSetting(const SettingsFile& file,
															  const SettingPath& sectionPath) const
	{
		return m_settings.at(file).at(sectionPath);
	}

	void SettingDefinitionMgr::setSetting(const SettingsFile& file,
										  const SettingPath& sectionPath,
										  const SettingDefinition& definition)
	{
		m_settings[file][sectionPath] = definition;
		m_settings[file][sectionPath].id = m_settings[file].size();
	}

	SecurityKey SettingDefinitionMgr::getSettingsFileEncryptionKey(const SettingsFile& file) const
	{
		return m_settingsFileEncryptionKey.at(file);
	}

	void SettingDefinitionMgr::setSettingsFileEncryptionKey(const SettingsFile& file,
															const SecurityKey& key)
	{
		m_settingsFileEncryptionKey[file] = key;
	}

	std::string SettingDefinitionMgr::getSettingsFolderPath() const
	{
		return m_settingsFolderPath;
	}

	void SettingDefinitionMgr::setSettingsFolderPath(const std::string& folderPath)
	{
		m_settingsFolderPath = folderPath;
	}

}}
