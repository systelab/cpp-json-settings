#pragma once

#include "JSONSettings/SettingsFile.h"
#include "JSONSettings/SettingPath.h"

#include <boost/optional.hpp>


namespace systelab { namespace setting { namespace rest_api {

	class SettingLookupHelper
	{
	public:
		static boost::optional<SettingPath> findById(const SettingsFile&, int settingId);
	};

}}}
