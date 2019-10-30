#pragma once

#include "SettingDefinition.h"


namespace systelab { namespace setting {

	class SettingDefinitionMgr
	{
	private:
		typedef std::string SettingsFile;
		typedef std::string SettingPath;
		typedef std::map< SettingsFile, std::map<SettingPath, SettingDefinition> > SettingsContainer;

	public:
		SettingDefinitionMgr();
		~SettingDefinitionMgr();

		static SettingDefinitionMgr& get();

		bool hasFile(const SettingsFile&) const;
		bool hasSetting(const SettingsFile&, const SettingPath&) const;
		const SettingDefinition& getSetting(const SettingsFile&, const SettingPath&) const;
		void setSetting(const SettingsFile&, const SettingPath&, const SettingDefinition&);

		std::string getSettingsFolderPath() const;
		void setSettingsFolderPath(const std::string&);

	private:
		SettingsContainer m_settings;
		std::string m_settingsFolderPath;
	};

}}
