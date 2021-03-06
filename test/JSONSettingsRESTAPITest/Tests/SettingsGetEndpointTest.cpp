#include "stdafx.h"
#include "SettingsEndpointBaseTest.h"

#include "JSONSettingsRESTAPI/Endpoints/SettingsGetEndpoint.h"

#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"


using namespace testing;
using namespace systelab::json::test_utility;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsGetEndpointTest : public SettingsEndpointBaseTest
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
			return std::make_unique<SettingsGetEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		std::unique_ptr<systelab::rest_api_core::IEndpoint> buildEndpointWithoutEncryptionAdapter(const SettingsFile& file)
		{
			auto settingsService = std::make_unique<systelab::setting::SettingsService>();
			return std::make_unique<SettingsGetEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		rest_api_core::EndpointRequestData buildHappyPathEndpointRequestData(int settingId)
		{
			rest_api_core::EndpointRequestData endpointRequestData;
			endpointRequestData.setParameters(rest_api_core::EndpointRequestParams({}, { {"id", settingId} }));
			return endpointRequestData;
		}

		std::string buildSettingExpectedContent(int settingId, const std::string& path,
												const std::string& typeName, bool useCache,
												const std::string& defaultValue,
												const std::string& currentValue)
		{
			std::stringstream expectedContentStream;
			expectedContentStream << "{" << std::endl;
			expectedContentStream << "    \"id\": " << settingId << "," << std::endl;
			expectedContentStream << "    \"path\": \"" << path << "\"," << std::endl;
			expectedContentStream << "    \"type\": \"" << typeName << "\"," << std::endl;
			expectedContentStream << "    \"useCache\": " << (useCache ? "true" : "false") << "," << std::endl;
			expectedContentStream << "    \"defaultValue\": \"" << defaultValue << "\"," << std::endl;
			expectedContentStream << "    \"currentValue\": \"" << currentValue << "\"" << std::endl;
			expectedContentStream << "}" << std::endl;

			return expectedContentStream.str();
		}
	};


	// Happy path - Not encrypted file - Integer setting
	TEST_F(SettingsGetEndpointTest, testExecuteForIntSettingOfMySettingsFileWhenFileDoesNotExist)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1)); // IntSetting has id=1

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "IntSettingCache", "integer", true, "1234", "1234"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForIntSettingOfMySettingsFileWhenFileExists)
	{
		writeMySettingsFile(54321, 9.87654, "XYZ", true);

		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1)); // IntSetting has id=1

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "IntSettingCache", "integer", true, "1234", "54321"),
								 reply->getContent(), m_jsonAdapter));
	}


	// Happy path - Not encrypted file - Double setting
	TEST_F(SettingsGetEndpointTest, testExecuteForDblSettingOfMySettingsFileWhenFileDoesNotExist)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(2)); // DblSetting has id=2

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(2, "DblSettingNoCache", "double", false, "5.678", "5.678"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForDblSettingOfMySettingsFileWhenFileExists)
	{
		writeMySettingsFile(54321, 9.87654, "XYZ", true);

		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(2)); // DblSetting has id=2

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(2, "DblSettingNoCache", "double", false, "5.678", "9.87654"),
								 reply->getContent(), m_jsonAdapter));
	}


	// Happy path - Not encrypted file - String setting
	TEST_F(SettingsGetEndpointTest, testExecuteForStrSettingOfMySettingsFileWhenFileDoesNotExist)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(3)); // StrSetting has id=3

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(3, "Section.StrSettingCache", "string", true, "ba", "ba"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForStrSettingOfMySettingsFileWhenFileExists)
	{
		writeMySettingsFile(54321, 9.87654, "XYZ", true);

		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(3)); // StrSetting has id=3

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(3, "Section.StrSettingCache", "string", true, "ba", "XYZ"),
								 reply->getContent(), m_jsonAdapter));
	}


	// Happy path - Not encrypted file - Boolean setting
	TEST_F(SettingsGetEndpointTest, testExecuteForBoolSettingOfMySettingsFileWhenFileDoesNotExist)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(4)); // Boolean setting has id=4

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(4, "Section.Subsection.BoolSettingNoCache", "boolean", false, "false", "false"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForBoolSettingOfMySettingsFileWhenFileExists)
	{
		writeMySettingsFile(54321, 9.87654, "XYZ", true);

		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(4)); // Boolean setting has id=4

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(4, "Section.Subsection.BoolSettingNoCache", "boolean", false, "false", "true"),
								 reply->getContent(), m_jsonAdapter));
	}


	// Happy path - Encrypted file
	TEST_F(SettingsGetEndpointTest, testExecuteForFirstSettingOfEncryptedSettingsFileWhenFileDoesNotExist)
	{
		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "Section.IntSettingCache", "integer", true, "9999", "9999"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForFirstSettingOfEncryptedSettingsFileWhenFileExists)
	{
		writeEncryptedSettingsFile(6666, 6.6, "6G6", false);

		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1)); // IntSetting has id=1

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "Section.IntSettingCache", "integer", true, "9999", "6666"),
								 reply->getContent(), m_jsonAdapter));
	}


	// Error cases
	TEST_F(SettingsGetEndpointTest, testExecuteForNotDefinedSettingsFileReturnsNotFoundReply)
	{
		auto endpoint = buildEndpoint("NotExistingFile");
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::NOT_FOUND, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForNotExistingSettingInSettingsFileReturnsNotFoundReply)
	{
		unsigned int notExistingSettingId = 666;
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(notExistingSettingId));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::NOT_FOUND, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForEncryptedFileWithoutEncryptionAdapterReturnsInternalServerErrorReply)
	{
		auto endpoint = buildEndpointWithoutEncryptionAdapter(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Unable to access encrypted settings file when no encryption adapter provided."),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsGetEndpointTest, testExecuteForRequestThatDoesNotHaveIdNumericParameterReturnsInternalServerErrorReply)
	{
		auto endpoint = buildEndpointWithoutEncryptionAdapter(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Configured endpoint route lacks 'id' numeric parameter."),
								 reply->getContent(), m_jsonAdapter));
	}

}}}}

