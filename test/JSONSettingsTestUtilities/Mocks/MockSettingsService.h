#pragma once

#include "JSONSettings/Services/ISettingsService.h"


namespace systelab { namespace setting { namespace test_utility {

	class MockSettingsService : public ISettingsService
	{
	public:
		MockSettingsService();
		virtual ~MockSettingsService();

		MOCK_CONST_METHOD3(getSettingInteger, int(const std::string&, const std::string&, int));
		MOCK_CONST_METHOD3(getSettingBoolean, int(const std::string&, const std::string&, bool));
		MOCK_CONST_METHOD3(getSettingString, std::string(const std::string&, const std::string&, const std::string&));

		MOCK_METHOD3(setSettingInteger, int(const std::string&, const std::string&, int));
		MOCK_METHOD3(setSettingBoolean, int(const std::string&, const std::string&, bool));
		MOCK_METHOD3(setSettingString, int(const std::string&, const std::string&, const std::string&));

		MOCK_METHOD3(clearCache, void());
	};

}}}

