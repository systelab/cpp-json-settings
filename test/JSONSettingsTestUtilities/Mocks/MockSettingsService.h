#pragma once

#include "JSONSettings/ISettingsService.h"


namespace systelab { namespace setting { namespace test_utility {

	class MockSettingsService : public ISettingsService
	{
	public:
		MockSettingsService();
		virtual ~MockSettingsService();

		MOCK_CONST_METHOD2(getSettingInteger, int(const std::string&, const std::string&));
		MOCK_CONST_METHOD2(getSettingBoolean, int(const std::string&, const std::string&));
		MOCK_CONST_METHOD2(getSettingString, std::string(const std::string&, const std::string&));

		MOCK_METHOD3(setSettingInteger, int(const std::string&, const std::string&, int));
		MOCK_METHOD3(setSettingBoolean, int(const std::string&, const std::string&, bool));
		MOCK_METHOD3(setSettingString, int(const std::string&, const std::string&, const std::string&));

		MOCK_METHOD0(clearCache, void());
	};

}}}

