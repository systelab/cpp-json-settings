#include "stdafx.h"
#include "JSONSettings/Services/FileIOService.h"

#include <boost/filesystem.hpp>


namespace systelab { namespace setting { namespace unit_test {

	class FileIOServiceTest : public ::testing::Test
	{
	public:
		void SetUp()
		{
			auto folderPath = boost::filesystem::path("FileIOServiceTest");
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			m_workingFilepath = boost::filesystem::absolute(folderPath) / "TestFile.txt";
			if (boost::filesystem::exists(m_workingFilepath))
			{
				boost::filesystem::remove(m_workingFilepath);
			}

			m_fileIOService = std::make_unique<FileIOService>();
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_workingFilepath))
			{
				boost::filesystem::remove(m_workingFilepath);
			}
		}

		void writeFile(const std::string& fileContents)
		{
			boost::filesystem::remove(m_workingFilepath);

			std::ofstream fileStream;
			fileStream.open(m_workingFilepath.string());
			fileStream << fileContents;
			fileStream.close();
		}

		std::string readFile()
		{
			std::string fileContents;
			std::ifstream ifs(m_workingFilepath.string());
			if (ifs)
			{
				std::stringstream ss;
				ss << ifs.rdbuf();
				fileContents = ss.str();
				ifs.close();
			}

			return fileContents;
		}

	protected:
		boost::filesystem::path m_workingFilepath;
		std::unique_ptr<FileIOService> m_fileIOService;
	};


	// Read
	TEST_F(FileIOServiceTest, testReadReturnsFileContentsWhenFileExists)
	{
		std::string fileContents = "This is my wonderful contents.";
		writeFile(fileContents);

		auto readFileContents = m_fileIOService->read(m_workingFilepath.string(), []() { return ""; });
		ASSERT_TRUE(readFileContents.is_initialized());
		EXPECT_EQ(fileContents, (*readFileContents).str());
	}

	TEST_F(FileIOServiceTest, testReadReturnsNullWhenFileDoesNotExist)
	{
		ASSERT_FALSE(m_fileIOService->read(m_workingFilepath.string(), []() { return ""; }).is_initialized());
	}

	TEST_F(FileIOServiceTest, testReadThrowsExceptionWhenEncryptionKeyIsNotEmpty)
	{
		ASSERT_THROW(m_fileIOService->read(m_workingFilepath.string(), []() { return "NonEmptyEncryptionKey"; }), IFileIOService::NoEncryptionAdapterException);
	}


	// Write
	TEST_F(FileIOServiceTest, testWriteSetsGivenBufferIntoWorkingFile)
	{
		std::string fileContents = "This is the new file contents.";
		std::stringstream fileContentsStream;
		fileContentsStream << fileContents;
		m_fileIOService->write(m_workingFilepath.string(), []() { return ""; }, *fileContentsStream.rdbuf());

		ASSERT_EQ(fileContents, readFile());
	}

	TEST_F(FileIOServiceTest, testWriteReplacesGivenBufferByPreviousContentsOfWorkingFile)
	{
		writeFile("Initial working file contents");

		std::string newFileContents = "This is the new file contents.";
		std::stringstream newFileContentsStream;
		newFileContentsStream << newFileContents;
		m_fileIOService->write(m_workingFilepath.string(), []() { return ""; }, *newFileContentsStream.rdbuf());

		ASSERT_EQ(newFileContents, readFile());
	}

	TEST_F(FileIOServiceTest, testWriteThrowsExceptionWhenEncryptionKeyIsNotEmpty)
	{
		std::stringstream fileContentsStream;
		ASSERT_THROW(m_fileIOService->write(m_workingFilepath.string(), []() { return "NonEmptyEncryptionKey"; }, *fileContentsStream.rdbuf()), IFileIOService::NoEncryptionAdapterException);
	}

}}}

