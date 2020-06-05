#pragma once

#include "IFileIOService.h"


namespace systelab { namespace setting {

	class FileIOService : public IFileIOService
	{
	public:
		FileIOService();
		virtual ~FileIOService();

		boost::optional<std::stringstream> read(const std::string& filepath, const SecurityKey&) const override;
		void write(const std::string& filepath, const SecurityKey&, std::streambuf& buffer) const override;
	};

}}

