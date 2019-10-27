#include "stdafx.h"
#include "SettingDefinitionMgr.h"


namespace systelab { namespace setting {

	SettingDefinitionMgr::SettingDefinitionMgr() = default;
	SettingDefinitionMgr::~SettingDefinitionMgr() = default;

	SettingDefinitionMgr& SettingDefinitionMgr::get()
	{
		static SettingDefinitionMgr instance;
		return instance;
	}

	const SettingDefinitionMgr::SettingsContainer& SettingDefinitionMgr::getSettingsMap() const
	{
		return m_settings;
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
	}

}}
