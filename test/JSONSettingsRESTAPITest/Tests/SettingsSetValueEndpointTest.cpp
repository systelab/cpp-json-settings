#include "stdafx.h"
#include "SettingsEndpointBaseTest.h"

#include "JSONSettingsRESTAPI/Endpoints/SettingsSetValueEndpoint.h"

#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"

#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"


using namespace testing;
using namespace systelab::json::test_utility;

namespace systelab { namespace setting { namespace rest_api { namespace unit_test {

	class SettingsSetValueEndpointTest : public SettingsEndpointBaseTest
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
			return std::make_unique<SettingsSetValueEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		std::unique_ptr<systelab::rest_api_core::IEndpoint> buildEndpointWithoutEncryptionAdapter(const SettingsFile& file)
		{
			auto settingsService = std::make_unique<systelab::setting::SettingsService>();
			return std::make_unique<SettingsSetValueEndpoint>(file, std::move(settingsService), m_jsonAdapter);
		}

		rest_api_core::EndpointRequestData buildHappyPathEndpointRequestData(int settingId, const std::string& newSettingValue)
		{
			std::stringstream ss;
			ss << "{";
			ss << "    \"newValue\": \"" << newSettingValue << "\"";
			ss << "}";

			return buildEndpointRequestData(settingId, ss.str());
		}

		rest_api_core::EndpointRequestData buildEndpointRequestData(int settingId, const std::string& requestContent)
		{
			rest_api_core::EndpointRequestData endpointRequestData;
			endpointRequestData.setParameters(rest_api_core::EndpointRequestParams({}, { {"id", settingId} }));
			endpointRequestData.setContent(requestContent);

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


	// Integer setting
	TEST_F(SettingsSetValueEndpointTest, testExecuteForIntSettingAndValidValueReturnsOKReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1, "9876")); // Integer setting has id=1

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "IntSettingCache", "integer", true, "1234", "9876"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForIntSettingAndValidValueWritesValueInMySettingsFile)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		endpoint->execute(buildHappyPathEndpointRequestData(1, "9876")); // Integer setting has id=1

		std::string expectedSettingsFileContent = "{ \"IntSettingCache\": \"9876\" }";
		EXPECT_TRUE(compareJSONs(expectedSettingsFileContent, *readSettingsFile(MySettingsFile::FILENAME), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForIntSettingAndInvalidValueReturnsBadRequestReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1, "ThisIsNotAnInteger")); // Integer setting has id=1

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::BAD_REQUEST, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("New setting value not valid."),
								 reply->getContent(), m_jsonAdapter));

		ASSERT_FALSE(readSettingsFile(MySettingsFile::FILENAME));
	}


	// Double setting
	TEST_F(SettingsSetValueEndpointTest, testExecuteForDblSettingAndValidValueReturnsOKReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(2, "98.7")); // Double setting has id=2

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(2, "DblSettingNoCache", "double", false, "5.678", "98.7"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForDblSettingAndValidValueWritesValueInMySettingsFile)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		endpoint->execute(buildHappyPathEndpointRequestData(2, "98.7")); // Double setting has id=2

		std::string expectedSettingsFileContent = "{ \"DblSettingNoCache\": \"98.7\" }";
		EXPECT_TRUE(compareJSONs(expectedSettingsFileContent, *readSettingsFile(MySettingsFile::FILENAME), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForDblSettingAndInvalidValueReturnsBadRequestReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(2, "ThisIsNotADouble")); // Double setting has id=2

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::BAD_REQUEST, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("New setting value not valid."),
								 reply->getContent(), m_jsonAdapter));

		ASSERT_FALSE(readSettingsFile(MySettingsFile::FILENAME));
	}


	// String setting
	TEST_F(SettingsSetValueEndpointTest, testExecuteForStrSettingAndValidValueReturnsOKReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(3, "MySuperNewValue")); // String setting has id=3

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(3, "Section.StrSettingCache", "string", true, "ba", "MySuperNewValue"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForStrSettingAndValidValueWritesValueInMySettingsFile)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		endpoint->execute(buildHappyPathEndpointRequestData(3, "MySuperNewValue")); // String setting has id=3

		std::string expectedSettingsFileContent = "{ \"Section\": { \"StrSettingCache\": \"MySuperNewValue\" } }";
		EXPECT_TRUE(compareJSONs(expectedSettingsFileContent, *readSettingsFile(MySettingsFile::FILENAME), m_jsonAdapter));
	}




	// Boolean setting


	// Encrypted setting
	TEST_F(SettingsSetValueEndpointTest, testExecuteForEncryptedSettingAndValidValueReturnsOKReply)
	{
		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1, "5555"));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::OK, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildSettingExpectedContent(1, "Section.IntSettingCache", "integer", true, "9999", "5555"),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForEncryptedSettingAndValidValueWritesValueInEncryptedSettingsFile)
	{
		auto endpoint = buildEndpoint(EncryptedSettingsFile::FILENAME);
		endpoint->execute(buildHappyPathEndpointRequestData(1, "5555"));

		std::string expectedSettingsFileContent = "{ \"Section\": { \"IntSettingCache\": \"5555\" } }";
		EXPECT_TRUE(compareJSONs(expectedSettingsFileContent, *readSettingsFile(EncryptedSettingsFile::FILENAME, EncryptedSettingsFile::ENCRYPTION_KEY), m_jsonAdapter));
	}


	// Error cases
	TEST_F(SettingsSetValueEndpointTest, testExecuteForNotDefinedSettingsFileReturnsNotFoundReply)
	{
		auto endpoint = buildEndpoint("NotExistingFile");
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1, "9876"));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::NOT_FOUND, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForNotExistingSettingInSettingsFileReturnsNotFoundReply)
	{
		unsigned int notExistingSettingId = 666;
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(notExistingSettingId, "9876"));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::NOT_FOUND, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs("{}", reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForEncryptedFileWithoutEncryptionAdapterReturnsInternalServerErrorReply)
	{
		auto endpoint = buildEndpointWithoutEncryptionAdapter(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildHappyPathEndpointRequestData(1, "9876"));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Unable to access encrypted settings file when no encryption adapter provided."),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForRequestThatDoesNotHaveIdNumericParameterReturnsInternalServerErrorReply)
	{
		auto endpoint = buildEndpoint(MySettingsFile::FILENAME);
		auto reply = endpoint->execute(rest_api_core::EndpointRequestData());

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Configured endpoint route lacks 'id' numeric parameter."),
								 reply->getContent(), m_jsonAdapter));
	}

	TEST_F(SettingsSetValueEndpointTest, testExecuteForRequestWhoseContentIsNotAValidJSONReturnsBadRequestReply)
	{
		auto endpoint = buildEndpointWithoutEncryptionAdapter(EncryptedSettingsFile::FILENAME);
		auto reply = endpoint->execute(buildEndpointRequestData(1, "This is not a JSON content"));

		ASSERT_TRUE(reply != nullptr);
		EXPECT_EQ(systelab::web_server::Reply::BAD_REQUEST, reply->getStatus());
		EXPECT_EQ("application/json", reply->getHeader("Content-Type"));
		EXPECT_TRUE(compareJSONs(buildExpectedMessageReplyContent("Request content not in JSON format."),
								 reply->getContent(), m_jsonAdapter));
	}

}}}}

