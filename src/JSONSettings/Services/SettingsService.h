#pragma once

#include "ISettingsService.h"

#include <boost/optional.hpp>

namespace systelab { namespace setting {

	struct SettingDefinition;
	struct SettingValue;

	class SettingsService : public ISettingsService
	{
	public:
		SettingsService();
		virtual ~SettingsService();

		int getSettingInteger(const std::string& filepath, const std::string& settingPath) const;
		bool getSettingBoolean(const std::string& filepath, const std::string& settingPath) const;
		std::string getSettingString(const std::string& filepath, const std::string& settingPath) const;

		void setSettingInteger(const std::string& filepath, const std::string& settingPath, int value);
		void setSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value);
		void setSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value);

		void clearCache();

	private:
		template<typename Type>
		Type getSetting(const std::string& filepath, const std::string& settingPath) const;

		template<typename Type>
		void setSetting(const std::string& filepath, const std::string& settingPath, Type defaultValue);

		template<typename Type>
		boost::optional<Type> getSettingFromCache(const std::string& filepath, const std::string& settingPath) const;

		template<typename Type>
		void setSettingIntoCache(const std::string& filepath, const std::string& settingPath, const Type& value) const;

		template<typename Type>
		Type getSettingValue(const SettingValue&) const;

		template<typename Type>
		const SettingDefinition& getSettingDefinition(const std::string& filepath,
													  const std::string& settingPath) const;
	};

}};
