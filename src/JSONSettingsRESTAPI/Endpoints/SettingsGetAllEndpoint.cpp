#include "stdafx.h"
#include "SettingsGetAllEndpoint.h"

#include "JSONSettings/ISettingsService.h"
#include "JSONSettings/SettingDefinitionMgr.h"

#include "JSONAdapterInterface/IJSONAdapter.h"


namespace systelab { namespace setting { namespace rest_api {

	SettingsGetAllEndpoint::SettingsGetAllEndpoint(const std::string& settingsFile,
												   std::unique_ptr<ISettingsService> settingsService,
												   const systelab::json::IJSONAdapter& jsonAdapter)
		:m_settingsFile(settingsFile)
		,m_settingsService(std::move(settingsService))
		,m_jsonAdapter(jsonAdapter)
	{
	}

	SettingsGetAllEndpoint::~SettingsGetAllEndpoint() = default;

	std::unique_ptr<systelab::web_server::Reply> SettingsGetAllEndpoint::execute(const systelab::rest_api_core::EndpointRequestData&)
	{
		//if (SettingDefinitionMgr::get().hasFile(static_cast<SettingDefinitionMgr::Set>settingFile))
		//{

		//}

		//if (m)

		throw "Not implemented";
	}
	
}}}
