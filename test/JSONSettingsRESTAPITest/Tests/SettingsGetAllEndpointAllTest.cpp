#include "stdafx.h"
#include "SettingsEndpointBaseTest.h"

#include "JSONSettingsRESTAPI/Endpoints/SettingsGetAllEndpoint.h"

#include "JSONSettings/SettingsService.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"

using namespace testing;
using namespace systelab::json::test_utility;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsGetAllEndpointTest : public SettingsEndpointBaseTest
	{
	public:
		void SetUp()
		{
			SettingsEndpointBaseTest::SetUp();
		}

		void TearDown()
		{
			SettingsEndpointBaseTest::TearDown();
		}

		std::unique_ptr<systelab::rest_api_core::IEndpoint> buildEndpoint(const SettingsFile& file)
		{
			auto settingsService = std::make_unique<systelab::setting::SettingsService>(m_encryptionAdapter);
			return std::make_unique<SettingsGetAllEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		std::unique_ptr<systelab::rest_api_core::IEndpoint> buildEndpointWithoutEncryptionAdapter(const SettingsFile& file)
		{
			auto settingsService = std::make_unique<systelab::setting::SettingsService>();
			return std::make_unique<SettingsGetAllEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		std::string buildMySettingsFileExpectedContent(const std::string& intSetting,
													   const std::string& dblSetting,
													   const std::string& strSetting,
													   const std::string& boolSetting)
		{
			std::stringstream expectedContentStream;
			expectedContentStream << "{" << std::endl;
			expectedContentStream << "    \"settings\": [{" << std::endl;
			expectedContentStream << "         \"id\": 1," << std::endl;
			expectedContentStream << "         \"path\": \"IntSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"integer\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"1234\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << intSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 2," << std::endl;
			expectedContentStream << "         \"path\": \"DblSettingNoCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"double\"," << std::endl;
			expectedContentStream << "         \"useCache\": false," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"5.678\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << dblSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 3," << std::endl;
			expectedContentStream << "         \"path\": \"Section.StrSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"string\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"ba\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << strSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 4," << std::endl;
			expectedContentStream << "         \"path\": \"Section.Subsection.BoolSettingNoCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"boolean\"," << std::endl;
			expectedContentStream << "         \"useCache\": false," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"false\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << boolSetting << "\"" << std::endl;
			expectedContentStream << "      }" << std::endl;
			expectedContentStream << "   ]" << std::endl;
			expectedContentStream << "}";

			return expectedContentStream.str();
		}

	protected:
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
		systelab::encryption::caeser_cypher::EncryptionAdapter m_encryptionAdapter;
	};

	TEST_F(SettingsGetAllEndpointTest, testExecuteForMySettingsFileWhenNoFileOnDisk)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));

		auto expectedReplyContent = buildMySettingsFileExpectedContent("1234", "5.678", "ba", "false");
		EXPECT_TRUE(compareJSONs(expectedReplyContent, reply->getContent(), m_jsonAdapter));
	}

}}}}

