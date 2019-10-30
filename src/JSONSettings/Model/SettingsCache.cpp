#include "stdafx.h"
#include "SettingsCache.h"

#include <boost/lexical_cast.hpp>


namespace systelab { namespace setting {

	SettingsCache::SettingsCache() = default;
	SettingsCache::~SettingsCache() = default;

	SettingsCache& SettingsCache::get()
	{
		static SettingsCache instance;
		return instance;
	}

	template<typename Type>
	bool SettingsCache::hasSetting(const SettingsFile& file,
								   const SettingPath& sectionPath) const
	{
		if (m_cache.find(file) != m_cache.end())
		{
			const auto& fileSettings = m_cache.at(file);
			return fileSettings.find(sectionPath) != fileSettings.end();
		}

		return false;
	}

	template <typename Type>
	Type SettingsCache::getSetting(const SettingsFile& file,
								   const SettingPath& sectionPath) const
	{
		auto itFile = m_cache.find(file);
		if (itFile != m_cache.end())
		{
			auto itSetting = itFile->second.find(sectionPath);
			if (itSetting != itFile->second.end())
			{
				return boost::lexical_cast<Type>(itSetting->second);
			}
		}

		throw std::runtime_error("Setting '" + settingPath + "' of file '" + file + "' not in cache");
	}

	template <typename Type>
	void SettingsCache::setSetting(const SettingsFile& file,
								   const SettingPath& sectionPath,
								   const Type& value)
	{
		std::stringstream oss;
		oss << value;
		m_cache[file][sectionPath] = oss.str();
	}

	void SettingsCache::clear()
	{
		m_cache.clear();
	}

}}
