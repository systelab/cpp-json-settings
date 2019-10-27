#pragma once

#include "SettingValue.h"


namespace systelab { namespace setting {

	struct SettingDefinition
	{
		bool cache;
		SettingValue defaultValue;
		std::function<bool(SettingValue)> validationFunction;
	};

}}
