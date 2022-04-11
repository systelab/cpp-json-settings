#include "stdafx.h"
#include "EncryptedFileIOService.h"

#include "EncryptionAdapterInterface/IEncryptionAdapter.h"


namespace systelab { namespace setting {

	EncryptedFileIOService::EncryptedFileIOService(const systelab::encryption::IEncryptionAdapter& encryptionAdapter)
		:m_encryptionAdapter(encryptionAdapter)
	{
	}

	EncryptedFileIOService::~EncryptedFileIOService() = default;

	boost::optional<std::stringstream> EncryptedFileIOService::read(const std::string& filepath, const SecurityKey& encryptionKey) const
	{
		std::ifstream inputFileStream(filepath, std::ios::binary);
		if (!inputFileStream)
		{
			return boost::none;
		}

		std::stringstream encryptedFileStream;
		encryptedFileStream << inputFileStream.rdbuf();
		inputFileStream.close();

		try
		{
			return std::stringstream(m_encryptionAdapter.decryptString(encryptionKey, encryptedFileStream.str()));
		}
		catch (systelab::encryption::IEncryptionAdapter::Exception&)
		{
			return boost::none;
		}
	}

	void EncryptedFileIOService::write(const std::string& filepath, const SecurityKey& encryptionKey, std::streambuf& buffer) const
	{
		std::stringstream ss;
		ss << &buffer;
		std::stringstream out(m_encryptionAdapter.encryptString(encryptionKey, ss.str()));
		std::string outputStr = out.str();

		std::ofstream outputFileStream(filepath, std::ofstream::binary);
		outputFileStream << outputStr;
		outputFileStream.close();
	}

}}
