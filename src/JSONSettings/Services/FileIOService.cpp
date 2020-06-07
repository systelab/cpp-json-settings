#include "stdafx.h"
#include "FileIOService.h"


namespace systelab { namespace setting {

	FileIOService::FileIOService() = default;
	FileIOService::~FileIOService() = default;

	boost::optional<std::stringstream> FileIOService::read(const std::string& filepath, const SecurityKey& encryptionKey) const
	{
		if (encryptionKey().size() > 0)
		{
			throw NoEncryptionAdapterException();
		}

		std::ifstream ifs(filepath);
		if (ifs)
		{
			std::stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();
			return ss;
		}
		else
		{
			return boost::none;
		}
	}

	void FileIOService::write(const std::string& filepath, const SecurityKey& encryptionKey, std::streambuf& buffer) const
	{
		if (encryptionKey().size() > 0)
		{
			throw NoEncryptionAdapterException();
		}

		std::ofstream f;
		f.exceptions(~std::ofstream::goodbit);
		f.open(filepath, std::ios_base::out);
		f << &(buffer);
		f.close();
	}

}}
