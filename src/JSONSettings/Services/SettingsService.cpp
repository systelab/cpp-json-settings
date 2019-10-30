#include "stdafx.h"
#include "SettingsService.h"

#include "Model/SettingsCache.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace systelab { namespace setting {

	SettingsService::SettingsService() = default;
	SettingsService::~SettingsService() = default;

	int SettingsService::getSettingInteger(const std::string& filepath,
											const std::string& settingPath,
											int defaultValue) const
	{
		return getSetting<int>(filepath, settingPath, defaultValue);
	}

	bool SettingsService::getSettingBoolean(const std::string& filepath,
											 const std::string& settingPath,
											 bool defaultValue) const
	{
		return getSetting<bool>(filepath, settingPath, defaultValue);
	}

	std::string SettingsService::getSettingString(const std::string& filepath,
												   const std::string& settingPath,
												   const std::string& defaultValue) const
	{
		return getSetting<std::string>(filepath, settingPath, defaultValue);
	}

	void SettingsService::setSettingInteger(const std::string& filepath,
											  const std::string& settingPath,
											  int value)
	{
		setSetting<int>(filepath, settingPath, value);
	}

	void SettingsService::setSettingBoolean(const std::string& filepath,
											  const std::string& settingPath,
											  bool value)
	{
		setSetting<bool>(filepath, settingPath, value);
	}

	void SettingsService::setSettingString(const std::string& filepath,
											 const std::string& settingPath,
											 const std::string& value)
	{
		setSetting<std::string>(filepath, settingPath, value);
	}

	template<typename Type>
	Type SettingsService::getSetting(const std::string& filepath,
									  const std::string& settingPath,
									  const Type& defaultValue) const
	{
		boost::optional<Type> cacheValue = getSettingFromCache<Type>(filepath, settingPath);
		if (cacheValue)
		{
			return *cacheValue;
		}

		Type value;
		try
		{
			std::ifstream ifs(filepath);
			if (!ifs)
			{
				return defaultValue;
			}

			std::stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();

			boost::property_tree::ptree tree;
			boost::property_tree::json_parser::read_json(ss, tree);
			value = tree.get<Type>(settingPath, defaultValue);
		}
		catch (boost::property_tree::json_parser_error& /*e*/)
		{
			value = defaultValue;
		}

		setSettingIntoCache<Type>(filepath, settingPath, value);

		return value;
	}

	template<typename Type>
	void SettingsService::setSetting(const std::string& filepath,
									   const std::string& settingPath,
									   Type value)
	{
		boost::property_tree::ptree tree;

		try
		{
			std::ifstream ifs(filepath);
			if (!ifs)
			{
				return;
			}

			std::stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();

			boost::property_tree::json_parser::read_json(ss, tree);
		}
		catch (boost::property_tree::json_parser_error& /*e*/)
		{
		}

		try
		{
			tree.put<Type>(settingPath, value);

			std::stringstream oss;
			boost::property_tree::json_parser::write_json(oss, tree);

			std::ofstream f;
			f.exceptions(~std::ofstream::goodbit);
			f.open(filepath, std::ios_base::out);
			f << &(*oss.rdbuf());
			f.close();
		}
		catch (boost::property_tree::json_parser_error& /*e*/)
		{
		}

		setSettingIntoCache<Type>(filepath, settingPath, value);
	}

	template<typename Type>
	boost::optional<Type> SettingsService::getSettingFromCache(const std::string& filepath,
															   const std::string& settingPath) const
	{
		if (SettingsCache::get().hasSetting<Type>(filepath, settingPath))
		{
			return SettingsCache::get().getSetting<Type>(filepath, settingPath);
		}
		else
		{
			return boost::none;
		}
	}

	template<typename Type>
	void SettingsService::setSettingIntoCache(const std::string& filepath,
											  const std::string& settingPath,
											  const Type& value) const
	{
		SettingsCache::get().setSetting<Type>(filepath, settingPath, value);
	}

}}
