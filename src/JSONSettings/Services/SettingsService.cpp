#include "stdafx.h"
#include "SettingsService.h"

#include "Model/SettingDefinitionMgr.h"
#include "Model/SettingsCache.h"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <type_traits>


namespace systelab { namespace setting {

	SettingsService::SettingsService() = default;
	SettingsService::~SettingsService() = default;

	int SettingsService::getSettingInteger(const std::string& filepath,
										   const std::string& settingPath) const
	{
		return getSetting<int>(filepath, settingPath);
	}

	bool SettingsService::getSettingBoolean(const std::string& filepath,
											const std::string& settingPath) const
	{
		return getSetting<bool>(filepath, settingPath);
	}

	std::string SettingsService::getSettingString(const std::string& filepath,
												  const std::string& settingPath) const
	{
		return getSetting<std::string>(filepath, settingPath);
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

	void SettingsService::clearCache()
	{
		SettingsCache::get().clear();
	}

	template<typename Type>
	Type SettingsService::getSetting(const std::string& filepath,
									 const std::string& settingPath) const
	{
		const SettingDefinition& definition = getSettingDefinition<Type>(filepath, settingPath);
		if (definition.useCache)
		{
			boost::optional<Type> cacheValue = getSettingFromCache<Type>(filepath, settingPath);
			if (cacheValue)
			{
				return *cacheValue;
			}
		}

		Type value;
		try
		{
			std::ifstream ifs(filepath);
			if (!ifs)
			{
				return getSettingValue<Type>(definition.defaultValue);
			}

			std::stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();

			boost::property_tree::ptree tree;
			boost::property_tree::json_parser::read_json(ss, tree);
			value = tree.get<Type>(settingPath, getSettingValue<Type>(definition.defaultValue));
		}
		catch (boost::property_tree::json_parser_error& /*e*/)
		{
			value = getSettingValue<Type>(definition.defaultValue);
		}

		if (definition.useCache)
		{
			setSettingIntoCache<Type>(filepath, settingPath, value);
		}

		return value;
	}

	template<typename Type>
	void SettingsService::setSetting(const std::string& filepath,
									 const std::string& settingPath,
									 Type value)
	{
		boost::property_tree::ptree tree;
		const SettingDefinition& definition = getSettingDefinition<Type>(filepath, settingPath);

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

		if (definition.useCache)
		{
			setSettingIntoCache<Type>(filepath, settingPath, value);
		}
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

	template<typename Type>
	Type SettingsService::getSettingValue(const SettingValue& settingValue) const
	{
		return boost::lexical_cast<Type>(settingValue.value);
	}

	template<typename Type>
	const SettingDefinition& SettingsService::getSettingDefinition(const std::string& filepath,
																   const std::string& settingPath) const
	{
		if (!SettingDefinitionMgr::get().hasSetting(filepath, settingPath))
		{
			throw std::runtime_error("Undefined setting '" + settingPath + "' for file '" + filepath + "'");
		}

		const SettingDefinition& definition = SettingDefinitionMgr::get().getSetting(filepath, settingPath);
		bool validType = ((std::is_same<int, Type>::value == true) && (definition.defaultValue.type == SettingValueType::IntValue)) ||
						  ((std::is_same<bool, Type>::value == true) && (definition.defaultValue.type == SettingValueType::BooleanValue)) ||
						  ((std::is_same<std::string, Type>::value == true) && (definition.defaultValue.type == SettingValueType::StringValue));
		if (!validType)
		{
			throw std::runtime_error("Wrong type for setting '" + settingPath + "' of file '" + filepath + "'");
		}

		return definition;
	}

}}
