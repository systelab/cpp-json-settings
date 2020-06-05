#include "stdafx.h"
#include "EncryptedFileIOService.h"

#include "FileIOService.h"

#include "EncryptionAdapterInterface/IEncryptionAdapter.h"


namespace systelab { namespace setting {

	EncryptedFileIOService::EncryptedFileIOService(const systelab::encryption::IEncryptionAdapter& encryptionAdapter)
		:m_encryptionAdapter(encryptionAdapter)
		,m_fileIOService(std::make_unique<FileIOService>())
	{
	}

	EncryptedFileIOService::~EncryptedFileIOService() = default;

	boost::optional<std::stringstream> EncryptedFileIOService::read(const std::string& filepath, const SecurityKey& encryptionKey) const
	{
		boost::optional<std::stringstream> encryptedFileStream = m_fileIOService->read(filepath, []() { return ""; });
		if (!encryptedFileStream)
		{
			return boost::none;
		}

		try
		{
			return std::stringstream(m_encryptionAdapter.decryptString(encryptionKey(), encryptedFileStream->str()));
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
		std::stringstream out(m_encryptionAdapter.encryptString(encryptionKey(), ss.str()));
		m_fileIOService->write(filepath, []() { return ""; }, *out.rdbuf());
	}

}}
