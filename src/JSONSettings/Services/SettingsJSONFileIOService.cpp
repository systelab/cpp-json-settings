#include "stdafx.h"
#include "SettingsJSONFileIOService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace systelab { namespace setting {

	SettingsJSONFileIOService::SettingsJSONFileIOService() = default;
	SettingsJSONFileIOService::~SettingsJSONFileIOService() = default;

	int SettingsJSONFileIOService::readSettingInteger(const std::string& filepath,
													  const std::string& settingPath,
													  int defaultValue) const
	{
		return readSetting<int>(filepath, settingPath, defaultValue);
	}

	bool SettingsJSONFileIOService::readSettingBoolean(const std::string& filepath,
													   const std::string& settingPath,
													   bool defaultValue) const
	{
		return readSetting<bool>(filepath, settingPath, defaultValue);
	}

	std::string SettingsJSONFileIOService::readSettingString(const std::string& filepath,
															 const std::string& settingPath,
															 const std::string& defaultValue) const
	{
		return readSetting<std::string>(filepath, settingPath, defaultValue);
	}

	void SettingsJSONFileIOService::writeSettingInteger(const std::string& filepath,
													const std::string& settingPath,
													int value)
	{
		writeSetting<int>(filepath, settingPath, value);
	}

	void SettingsJSONFileIOService::writeSettingBoolean(const std::string& filepath,
														const std::string& settingPath,
														bool value)
	{
		writeSetting<bool>(filepath, settingPath, value);
	}

	void SettingsJSONFileIOService::writeSettingString(const std::string& filepath,
													   const std::string& settingPath,
													   const std::string& value)
	{
		writeSetting<std::string>(filepath, settingPath, value);
	}

	template<typename Type>
	Type SettingsJSONFileIOService::readSetting(const std::string& filepath,
												const std::string& settingPath,
												const Type& defaultValue) const
	{
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

		return value;
	}

	template<typename Type>
	void SettingsJSONFileIOService::writeSetting(const std::string& filepath,
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
	}

}}
