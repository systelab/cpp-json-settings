#pragma once


namespace systelab { namespace setting {

	class ISettingsService
	{
	public:
		virtual ~ISettingsService() = default;

		virtual int getSettingInteger(const std::string& filepath, const std::string& settingPath) const = 0;
		virtual bool getSettingBoolean(const std::string& filepath, const std::string& settingPath) const = 0;
		virtual std::string getSettingString(const std::string& filepath, const std::string& settingPath) const = 0;

		virtual void setSettingInteger(const std::string& filepath, const std::string& settingPath, int value) = 0;
		virtual void setSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value) = 0;
		virtual void setSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value) = 0;

		virtual void clearCache() = 0;
	};

}};
