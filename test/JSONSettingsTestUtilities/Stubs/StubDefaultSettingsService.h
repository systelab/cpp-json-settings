#pragma once

#include "../Mocks/MockSettingsService.h"


namespace systelab { namespace setting { namespace test_utility {

	class StubDefaultSettingsService : public MockSettingsService
	{
	public:
		StubDefaultSettingsService();
		virtual ~StubDefaultSettingsService();

		virtual int getSettingIntegerStub(const std::string&, const std::string&);
		virtual double getSettingDoubleStub(const std::string&, const std::string&);
		virtual bool getSettingBooleanStub(const std::string&, const std::string&);
		virtual std::string getSettingString(const std::string&, const std::string&);
	};

}}}

