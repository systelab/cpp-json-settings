#include "stdafx.h"
#include "SettingsCache.h"


namespace systelab { namespace setting {

	SettingsCache::SettingsCache() = default;
	SettingsCache::~SettingsCache() = default;

	SettingsCache& SettingsCache::get()
	{
		static SettingsCache instance;
		return instance;
	}

	void SettingsCache::clear()
	{
		m_cache.clear();
	}

}}
