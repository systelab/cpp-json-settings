#pragma once

#include "SettingValue.h"


namespace systelab { namespace setting {

	struct SettingDefinition
	{
		int id;
		SettingValue defaultValue;
		std::function<bool(SettingValue)> validationFunction;
		bool useCache;
	};

}}
