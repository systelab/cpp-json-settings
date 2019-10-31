#pragma once

#include "SettingValue.h"


namespace systelab { namespace setting {

	class SettingsCache
	{
	private:
		typedef std::string SettingsFile;
		typedef std::string SettingPath;
		typedef std::map< SettingsFile, std::map<SettingPath, std::string> > Container;

	public:
		SettingsCache();
		~SettingsCache();

		static SettingsCache& get();

		template<typename Type>
		bool hasSetting(const SettingsFile&, const SettingPath&) const;

		template<typename Type>
		Type getSetting(const SettingsFile&, const SettingPath&) const;

		template<typename Type>
		void setSetting(const SettingsFile&, const SettingPath&, const Type&);

		void clear();

	private:
		Container m_cache;
	};

}}

#include "SettingsCache.inl"
