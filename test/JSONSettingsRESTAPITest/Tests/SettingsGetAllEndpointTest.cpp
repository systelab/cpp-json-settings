#include "stdafx.h"
#include "SettingsEndpointBaseTest.h"

#include "JSONSettingsRESTAPI/Endpoints/SettingsGetAllEndpoint.h"

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

		std::string buildEncryptedSettingsFileExpectedContent(const std::string& intSetting,
															  const std::string& dblSetting,
															  const std::string& strSetting,
															  const std::string& boolSetting)
		{
			std::stringstream expectedContentStream;
			expectedContentStream << "{" << std::endl;
			expectedContentStream << "    \"settings\": [{" << std::endl;
			expectedContentStream << "         \"id\": 1," << std::endl;
			expectedContentStream << "         \"path\": \"Section.IntSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"integer\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"9999\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << intSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 2," << std::endl;
			expectedContentStream << "         \"path\": \"Section.DblSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"double\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"9.9\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << dblSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 3," << std::endl;
			expectedContentStream << "         \"path\": \"Section.StrSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"string\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"99\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << strSetting << "\"" << std::endl;
			expectedContentStream << "      }, {" << std::endl;
			expectedContentStream << "         \"id\": 4," << std::endl;
			expectedContentStream << "         \"path\": \"Section.BoolSettingCache\"," << std::endl;
			expectedContentStream << "         \"type\": \"boolean\"," << std::endl;
			expectedContentStream << "         \"useCache\": true," << std::endl;
			expectedContentStream << "         \"defaultValue\": \"true\"," << std::endl;
			expectedContentStream << "         \"currentValue\": \"" << boolSetting << "\"" << std::endl;
			expectedContentStream << "      }" << std::endl;
			expectedContentStream << "   ]" << std::endl;
			expectedContentStream << "}";

			return expectedContentStream.str();
		}
	};


	// Happy path - Not encrypted file
	TEST_F(SettingsGetAllEndpointTest, testExecuteForMySettingsFileWhenFileDoesNotExistReturnsExpectedReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildMySettingsFileExpectedContent("1234", "5.678", "ba", "false"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetAllEndpointTest, testExecuteForMySettingsFileWhenFileExistsReturnsExpectedReply)
	{
		writeMySettingsFile(54321, 9.87654, "XYZ", true);

		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildMySettingsFileExpectedContent("54321", "9.87654", "XYZ", "true"), reply->getContent(), m_jsonAdapter));
	}


	// Happy path - Encrypted file
	TEST_F(SettingsGetAllEndpointTest, testExecuteForEncryptedSettingsFileWhenFileDoesNotExistReturnsExpectedReply)
	{
		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildEncryptedSettingsFileExpectedContent("9999", "9.9", "99", "true"), reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetAllEndpointTest, testExecuteForEncryptedSettingsFileWhenFileExistsReturnsExpectedReply)
	{
		writeEncryptedSettingsFile(6666, 6.6, "6G6", false);

		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildEncryptedSettingsFileExpectedContent("6666", "6.6", "6G6", "false"), reply->getContent(), m_jsonAdapter));
	}


	// Error cases
	TEST_F(SettingsGetAllEndpointTest, testExecuteForNotDefinedSettingsFileReturnsNotFoundReply)
	{
		auto endpoint = buildEndpoint("NotExistingFile");
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::NOT_FOUND, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetAllEndpointTest, testExecuteForEncryptedFileWithoutEncryptionAdapterReturnsInternalServerErrorReply)
	{
		auto endpoint = buildEndpointWithoutEncryptionAdapter(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Unable to access encrypted settings file when no encryption adapter provided."),
								 reply->getContent(), m_jsonAdapter));
	}

}}}}

