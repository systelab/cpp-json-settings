#include "stdafx.h"
#include "JSONSettings/Services/EncryptedFileIOService.h"

#include "CaeserCypherEncryptionAdapter/EncryptionAdapter.h"

#include <boost/filesystem.hpp>


namespace systelab { namespace setting { namespace unit_test {

	class EncryptedFileIOServiceTest : public ::testing::Test
	{
	public:
		void SetUp()
		{
			auto folderPath = boost::filesystem::path("EncryptedFileIOServiceTest");
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			m_workingFilepath = boost::filesystem::absolute(folderPath) / "TestFile.txt";
			if (boost::filesystem::exists(m_workingFilepath))
			{
				boost::filesystem::remove(m_workingFilepath);
			}

			m_encryptedFileIOService = std::make_unique<EncryptedFileIOService>(m_encryptionAdapter);

			m_encryptionKey = "MySecretKey";
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_workingFilepath))
			{
				boost::filesystem::remove(m_workingFilepath);
			}
		}

		void writeEncryptedFile(const std::string& fileContents)
		{
			boost::filesystem::remove(m_workingFilepath);

			std::ofstream fileStream;
			fileStream.open(m_workingFilepath.string());
			fileStream << m_encryptionAdapter.encryptString(m_encryptionKey, fileContents);
			fileStream.close();
		}

		std::string readEncryptedFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_workingFilepath.string());
			if (ifs)
			{
				std::stringstream ss;
				ss << ifs.rdbuf();
				fileContents = m_encryptionAdapter.decryptString(m_encryptionKey, ss.str());
				ifs.close();
			}

			return fileContents;
		}

	protected:
		std::unique_ptr<EncryptedFileIOService> m_encryptedFileIOService;
		systelab::encryption::caeser_cypher::EncryptionAdapter m_encryptionAdapter;

		std::string m_encryptionKey;
		boost::filesystem::path m_workingFilepath;
	};


	// Read
	TEST_F(EncryptedFileIOServiceTest, testReadReturnsDecryptedFileContentsWhenFileExistsAndEncryptionKeyIsValid)
	{
		std::string fileContents = "This is my wonderful contents.";
		writeEncryptedFile(fileContents);

		auto readFileContents = m_encryptedFileIOService->read(m_workingFilepath.string(), [this]() { return m_encryptionKey; });
		ASSERT_TRUE(readFileContents.is_initialized());
		EXPECT_EQ(fileContents, (*readFileContents).str());
	}

	TEST_F(EncryptedFileIOServiceTest, testReadReturnsNullWhenFileDoesNotExist)
	{
		ASSERT_FALSE(m_encryptedFileIOService->read(m_workingFilepath.string(), [this]() { return m_encryptionKey; }).is_initialized());
	}

	TEST_F(EncryptedFileIOServiceTest, testReadReturnsNullWhenFileExistsButEncryptionKeyIsInvalid)
	{
		ASSERT_FALSE(m_encryptedFileIOService->read(m_workingFilepath.string(), []() { return "InvalidEncryptionKey"; }).is_initialized());
	}


	// Write
	TEST_F(EncryptedFileIOServiceTest, testWriteSetsGivenBufferEncryptedIntoWorkingFile)
	{
		std::string fileContents = "This is the new file contents (encrypted).";
		std::stringstream fileContentsStream;
		fileContentsStream << fileContents;
		m_encryptedFileIOService->write(m_workingFilepath.string(), [this]() { return m_encryptionKey; }, *fileContentsStream.rdbuf());

		ASSERT_EQ(fileContents, readEncryptedFile());
	}

	TEST_F(EncryptedFileIOServiceTest, testWriteReplacesGivenBufferEncryptedByPreviousContentsOfWorkingFile)
	{
		writeEncryptedFile("Initial working file contents (encrypted).");

		std::string newFileContents = "This is the new file contents (encrypted).";
		std::stringstream newFileContentsStream;
		newFileContentsStream << newFileContents;
		m_encryptedFileIOService->write(m_workingFilepath.string(), [this]() { return m_encryptionKey; }, *newFileContentsStream.rdbuf());

		ASSERT_EQ(newFileContents, readEncryptedFile());
	}

}}}

