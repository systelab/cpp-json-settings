#pragma once

#include "SecurityKey.h"
#include "SettingDefinition.h"
#include "SettingsFile.h"
#include "SettingPath.h"


namespace systelab { namespace setting {

	class SettingDefinitionMgr
	{
	private:
		typedef std::map< SettingsFile, std::map<SettingPath, SettingDefinition> > SettingsContainer;

	public:
		SettingDefinitionMgr();
		~SettingDefinitionMgr();

		static SettingDefinitionMgr& get();

		bool hasFile(const SettingsFile&) const;
		std::map<SettingPath, SettingDefinition> getFileSettings(const SettingsFile&) const;

		bool hasSetting(const SettingsFile&, const SettingPath&) const;
		const SettingDefinition& getSetting(const SettingsFile&, const SettingPath&) const;
		void setSetting(const SettingsFile&, const SettingPath&, const SettingDefinition&);

		SecurityKey getSettingsFileEncryptionKey(const SettingsFile&) const;
		void setSettingsFileEncryptionKey(const SettingsFile&, const SecurityKey&);

		std::string getSettingsFolderPath() const;
		void setSettingsFolderPath(const std::string&);

	private:
		SettingsContainer m_settings;
		std::map<SettingsFile, SecurityKey> m_settingsFileEncryptionKey;
		std::string m_settingsFolderPath;
	};

}}
