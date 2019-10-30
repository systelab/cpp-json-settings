#pragma once

#include "ISettingsService.h"

#include <boost/optional.hpp>

namespace systelab { namespace setting {

	class SettingsService : public ISettingsService
	{
	public:
		SettingsService();
		virtual ~SettingsService();

		int getSettingInteger(const std::string& filepath, const std::string& settingPath, int defaultValue) const;
		bool getSettingBoolean(const std::string& filepath, const std::string& settingPath, bool defaultValue) const;
		std::string getSettingString(const std::string& filepath, const std::string& settingPath, const std::string& defaultValue) const;

		void setSettingInteger(const std::string& filepath, const std::string& settingPath, int value);
		void setSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value);
		void setSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value);

	private:
		template<typename Type>
		Type getSetting(const std::string& filepath, const std::string& settingPath, const Type& defaultValue) const;

		template<typename Type>
		void setSetting(const std::string& filepath, const std::string& settingPath, Type defaultValue);

		template<typename Type>
		boost::optional<Type> getSettingFromCache(const std::string& filepath, const std::string& settingPath) const;

		template<typename Type>
		void setSettingIntoCache(const std::string& filepath, const std::string& settingPath, const Type& value) const;
	};

}};
