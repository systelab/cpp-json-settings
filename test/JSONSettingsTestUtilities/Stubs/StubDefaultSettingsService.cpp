#include "stdafx.h"
#include "StubDefaultSettingsService.h"

#include "JSONSettings/SettingDefinitionMgr.h"


namespace systelab { namespace setting { namespace test_utility {

	StubDefaultSettingsService::StubDefaultSettingsService()
	{
		ON_CALL(*this, getSettingInteger(_, _)).WillByDefault(Invoke(&this, &StubDefaultSettingsService::getSettingIntegerStub));
		ON_CALL(*this, getSettingDouble(_, _)).WillByDefault(Invoke(&this, &StubDefaultSettingsService::getSettingDoubleStub));
		ON_CALL(*this, getSettingBoolean(_, _)).WillByDefault(Invoke(&this, &StubDefaultSettingsService::getSettingBooleanStub));
		ON_CALL(*this, getSettingString(_, _)).WillByDefault(Invoke(&this, &StubDefaultSettingsService::getSettingStringStub));
	}

	StubDefaultSettingsService::~StubDefaultSettingsService() = default;

	int StubDefaultSettingsService::getSettingIntegerStub(const std::string& filename, const std::string& settingPath)
	{
		if (SettingDefinitionMgr::get().hasSetting(filename, settingPath))
		{
			const auto& definition = SettingDefinitionMgr::get().getSetting(filename, settingPath);
			return std::atoi(definition.defaultValue.value.c_str());
		}
		else
		{
			throw std::runtime_error("Undefined setting '" + settingPath + "' for file '" + filename + "'");
		}
	}

	double StubDefaultSettingsService::getSettingDoubleStub(const std::string& filename, const std::string& settingPath)
	{
		if (SettingDefinitionMgr::get().hasSetting(filename, settingPath))
		{
			const auto& definition = SettingDefinitionMgr::get().getSetting(filename, settingPath);
			return std::atof(definition.defaultValue.value.c_str());
		}
		else
		{
			throw std::runtime_error("Undefined setting '" + settingPath + "' for file '" + filename + "'");
		}
	}

	bool StubDefaultSettingsService::getSettingBooleanStub(const std::string& filename, const std::string& settingPath)
	{
		if (SettingDefinitionMgr::get().hasSetting(filename, settingPath))
		{
			const auto& definition = SettingDefinitionMgr::get().getSetting(filename, settingPath);
			return (definition.defaultValue.value == "true");
		}
		else
		{
			throw std::runtime_error("Undefined setting '" + settingPath + "' for file '" + filename + "'");
		}
	}

	std::string StubDefaultSettingsService::getSettingStringStub(const std::string& filename, const std::string& settingPath)
	{
		if (SettingDefinitionMgr::get().hasSetting(filename, settingPath))
		{
			const auto& definition = SettingDefinitionMgr::get().getSetting(filename, settingPath);
			return definition.defaultValue.value;
		}
		else
		{
			throw std::runtime_error("Undefined setting '" + settingPath + "' for file '" + filename + "'");
		}
	}
		
}}}

