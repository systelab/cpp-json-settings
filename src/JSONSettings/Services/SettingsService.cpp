#include "stdafx.h"
#include "SettingsService.h"

#include "Model/SettingsCache.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace systelab { namespace setting {

	SettingsService::SettingsService() = default;
	SettingsService::~SettingsService() = default;

	int SettingsService::readSettingInteger(const std::string& filepath,
											const std::string& settingPath,
											int defaultValue) const
	{
		return readSetting<int>(filepath, settingPath, defaultValue);
	}

	bool SettingsService::readSettingBoolean(const std::string& filepath,
											 const std::string& settingPath,
											 bool defaultValue) const
	{
		return readSetting<bool>(filepath, settingPath, defaultValue);
	}

	std::string SettingsService::readSettingString(const std::string& filepath,
												   const std::string& settingPath,
												   const std::string& defaultValue) const
	{
		return readSetting<std::string>(filepath, settingPath, defaultValue);
	}

	void SettingsService::writeSettingInteger(const std::string& filepath,
											  const std::string& settingPath,
											  int value)
	{
		writeSetting<int>(filepath, settingPath, value);
	}

	void SettingsService::writeSettingBoolean(const std::string& filepath,
											  const std::string& settingPath,
											  bool value)
	{
		writeSetting<bool>(filepath, settingPath, value);
	}

	void SettingsService::writeSettingString(const std::string& filepath,
											 const std::string& settingPath,
											 const std::string& value)
	{
		writeSetting<std::string>(filepath, settingPath, value);
	}

	template<typename Type>
	Type SettingsService::readSetting(const std::string& filepath,
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
	void SettingsService::writeSetting(const std::string& filepath,
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
