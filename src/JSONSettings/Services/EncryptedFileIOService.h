#pragma once

#include "IFileIOService.h"


namespace systelab { namespace encryption {
	class IEncryptionAdapter;
}}

namespace systelab { namespace setting {

	class EncryptedFileIOService final : public IFileIOService
	{
	public:
		EncryptedFileIOService(const systelab::encryption::IEncryptionAdapter&);
		virtual ~EncryptedFileIOService();

		boost::optional<std::stringstream> read(const std::string& filepath, const SecurityKey&) const override;
		void write(const std::string& filepath, const SecurityKey&, std::streambuf& buffer) const override;

	private:
		const systelab::encryption::IEncryptionAdapter& m_encryptionAdapter;
	};

}}

