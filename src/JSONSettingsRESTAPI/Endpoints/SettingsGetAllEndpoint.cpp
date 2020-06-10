#include "stdafx.h"
#include "SettingsGetAllEndpoint.h"

#include "JSONSettings/ISettingsService.h"


namespace systelab { namespace setting { namespace rest_api {

	SettingsGetAllEndpoint::SettingsGetAllEndpoint(std::unique_ptr<ISettingsService> settingsService)
		:m_settingsService(std::move(settingsService))
	{
	}

	SettingsGetAllEndpoint::~SettingsGetAllEndpoint() = default;

	std::unique_ptr<systelab::web_server::Reply> SettingsGetAllEndpoint::execute(const systelab::rest_api_core::EndpointRequestData&)
	{
		throw "Not implemented";
	}
	
}}}
