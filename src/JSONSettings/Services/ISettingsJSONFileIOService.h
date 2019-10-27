#pragma once


namespace systelab { namespace setting {

	class ISettingsJSONFileIOService
	{
	public:
		virtual ~ISettingsJSONFileIOService() = default;

		virtual int readSettingInteger(const std::string& filepath, const std::string& settingPath, int defaultValue) const = 0;
		virtual bool readSettingBoolean(const std::string& filepath, const std::string& settingPath, bool defaultValue) const = 0;
		virtual std::string readSettingString(const std::string& filepath, const std::string& settingPath, const std::string& defaultValue) const = 0;

		virtual void writeSettingInteger(const std::string& filepath, const std::string& settingPath, int value) = 0;
		virtual void writeSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value) = 0;
		virtual void writeSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value) = 0;
	};

}};
