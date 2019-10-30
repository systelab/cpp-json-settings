#pragma once

#include "ISettingsService.h"

#include <boost/optional.hpp>

namespace systelab { namespace setting {

	class SettingsService : public ISettingsService
	{
	public:
		SettingsService();
		virtual ~SettingsService();

		int readSettingInteger(const std::string& filepath, const std::string& settingPath, int defaultValue) const;
		bool readSettingBoolean(const std::string& filepath, const std::string& settingPath, bool defaultValue) const;
		std::string readSettingString(const std::string& filepath, const std::string& settingPath, const std::string& defaultValue) const;

		void writeSettingInteger(const std::string& filepath, const std::string& settingPath, int value);
		void writeSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value);
		void writeSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value);

	private:
		template<typename Type>
		Type readSetting(const std::string& filepath, const std::string& settingPath, const Type& defaultValue) const;

		template<typename Type>
		void writeSetting(const std::string& filepath, const std::string& settingPath, Type defaultValue);

		template<typename Type>
		boost::optional<Type> getSettingFromCache(const std::string& filepath, const std::string& settingPath) const;

		template<typename Type>
		void setSettingIntoCache(const std::string& filepath, const std::string& settingPath, const Type& value) const;
	};

}};
