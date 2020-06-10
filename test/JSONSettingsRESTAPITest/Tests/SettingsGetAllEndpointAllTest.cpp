#include "stdafx.h"
#include "JSONSettingsRESTAPI/Endpoints/SettingsGetAllEndpoint.h"

#include "TestSettingsDefinition.h"

#include "JSONSettings/SettingsService.h"
#include "RapidJSONAdapter/JSONAdapter.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"


using namespace testing;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsGetAllEndpointTest : public Test
	{
	public:
		void SetUp()
		{
			auto settingsService = std::make_unique<systelab::setting::SettingsService>();
			m_endpoint = std::make_unique<SettingsGetAllEndpoint>("MySettingsFile.json", std::move(settingsService), m_jsonAdapter);
		}

	protected:
		std::unique_ptr<systelab::rest_api_core::IEndpoint> m_endpoint;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
	};

	TEST_F(SettingsGetAllEndpointTest, testGetSettingsReturnsValuesFromFile)
	{
		ASSERT_ANY_THROW(m_endpoint->execute(rest_api_core::EndpointRequestData()));
	}

}}}}

