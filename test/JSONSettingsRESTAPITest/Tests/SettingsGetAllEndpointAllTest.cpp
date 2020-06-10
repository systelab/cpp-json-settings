#include "stdafx.h"
#include "JSONSettingsRESTAPI/Endpoints/SettingsGetAllEndpoint.h"

#include "TestSettingsDefinition.h"


using namespace testing;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsGetAllEndpointTest : public Test
	{
	public:
		void SetUp()
		{
			auto settingsService = std::make_unique<SettingsService>();
			m_endpoint = std::make_unique<SettingsGetAllEndpoint>("MySettingsFile.json", std::move(settingsService), m_jsonAdapter);
		}

	private:
		std::unique_ptr<systelab::rest_api_core::IEndpoint> m_endpoint;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
	};

	TEST_F(SettingsGetAllEndpointTest, testGetSettingsReturnsValuesFromFile)
	{
		// TODO
	}

}}}}

