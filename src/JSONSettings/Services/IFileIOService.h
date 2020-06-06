#pragma once

#include <boost/optional.hpp>

#include "../Model/SecurityKey.h"


namespace systelab { namespace setting {

	class IFileIOService
	{
	public:
		virtual ~IFileIOService() = default;

		virtual boost::optional<std::stringstream> read(const std::string& filepath, const SecurityKey&) const = 0;
		virtual void write(const std::string& filepath, const SecurityKey&, std::streambuf& buffer) const = 0;

	public:
		struct Exception : std::runtime_error
		{
			Exception(const std::string& message)
				: std::runtime_error(message.c_str())
			{
			}
		};
	};

}}