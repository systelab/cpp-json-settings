#pragma once

#include "JSONSettings/ISettingsService.h"


namespace systelab { namespace setting { namespace test_utility {

	class MockSettingsService : public ISettingsService
	{
	public:
		MockSettingsService();
		virtual ~MockSettingsService();

		MOCK_CONST_METHOD2(getSettingInteger, int(const std::string&, const std::string&));
		MOCK_CONST_METHOD2(getSettingDouble, double(const std::string&, const std::string&));
		MOCK_CONST_METHOD2(getSettingBoolean, bool(const std::string&, const std::string&));
		MOCK_CONST_METHOD2(getSettingString, std::string(const std::string&, const std::string&));

		MOCK_METHOD3(setSettingInteger, void(const std::string&, const std::string&, int));
		MOCK_METHOD3(setSettingDouble, void(const std::string&, const std::string&, double));
		MOCK_METHOD3(setSettingBoolean, void(const std::string&, const std::string&, bool));
		MOCK_METHOD3(setSettingString, void(const std::string&, const std::string&, const std::string&));

		MOCK_METHOD0(clearCache, void());
	};


	#define ON_CALL_JSON_SETTING_INT(SERVICE, NAMESPACE, SETTING_NAME, RETURN_VALUE) \
		ON_CALL((SERVICE), getSettingInteger(NAMESPACE::FILENAME, NAMESPACE::SETTING_NAME::PATH)) \
			.WillByDefault(Return(RETURN_VALUE));

	#define ON_CALL_JSON_SETTING_DBL(SERVICE, NAMESPACE, SETTING_NAME, RETURN_VALUE) \
		ON_CALL((SERVICE), getSettingDouble(NAMESPACE::FILENAME, NAMESPACE::SETTING_NAME::PATH)) \
			.WillByDefault(Return(RETURN_VALUE));

	#define ON_CALL_JSON_SETTING_STR(SERVICE, NAMESPACE, SETTING_NAME, RETURN_VALUE) \
		ON_CALL((SERVICE), getSettingString(NAMESPACE::FILENAME, NAMESPACE::SETTING_NAME::PATH)) \
			.WillByDefault(Return(RETURN_VALUE));

	#define ON_CALL_JSON_SETTING_BOOL(SERVICE, NAMESPACE, SETTING_NAME, RETURN_VALUE) \
		ON_CALL((SERVICE), getSettingBoolean(NAMESPACE::FILENAME, NAMESPACE::SETTING_NAME::PATH)) \
			.WillByDefault(Return(RETURN_VALUE));

}}}

