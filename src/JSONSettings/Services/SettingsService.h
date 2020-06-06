#pragma once

#include "ISettingsService.h"

#include <boost/optional.hpp>


namespace systelab { namespace encryption {
	class IEncryptionAdapter;
}}

namespace systelab { namespace setting {

	class IFileIOService;
	struct SettingDefinition;
	struct SettingValue;

	class SettingsService : public ISettingsService
	{
	public:
		SettingsService();
		SettingsService(const systelab::encryption::IEncryptionAdapter&);
		virtual ~SettingsService();

		int getSettingInteger(const std::string& filename, const std::string& settingPath) const override;
		double getSettingDouble(const std::string& filename, const std::string& settingPath) const override;
		bool getSettingBoolean(const std::string& filename, const std::string& settingPath) const override;
		std::string getSettingString(const std::string& filename, const std::string& settingPath) const override;

		void setSettingInteger(const std::string& filepath, const std::string& settingPath, int value) override;
		void setSettingDouble(const std::string& filepath, const std::string& settingPath, double value) override;
		void setSettingBoolean(const std::string& filepath, const std::string& settingPath, bool value) override;
		void setSettingString(const std::string& filepath, const std::string& settingPath, const std::string& value) override;

		void clearCache() override;

	private:
		template<typename Type>
		Type getSetting(const std::string& filename, const std::string& settingPath) const;

		template<typename Type>
		void setSetting(const std::string& filename, const std::string& settingPath, Type defaultValue);

		template<typename Type>
		boost::optional<Type> getSettingFromCache(const std::string& filename, const std::string& settingPath) const;

		template<typename Type>
		void setSettingIntoCache(const std::string& filename, const std::string& settingPath, const Type& value) const;

		template<typename Type>
		Type getSettingValue(const SettingValue&) const;

		template<typename Type>
		const SettingDefinition& getSettingDefinition(const std::string& filename, const std::string& settingPath) const;

		std::string buildFilepath(const std::string& filename) const;

	private:
		std::unique_ptr<IFileIOService> m_fileIOService;
	};

}};
