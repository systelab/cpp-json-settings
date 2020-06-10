#include "stdafx.h"
#include "SettingsGetAllEndpoint.h"

#include "JSONSettings/ISettingsService.h"
#include "JSONSettings/SettingDefinitionMgr.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace setting { namespace rest_api {

	SettingsGetAllEndpoint::SettingsGetAllEndpoint(const SettingsFile& settingsFile,
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
		if (!SettingDefinitionMgr::get().hasFile(m_settingsFile))
		{
			auto reply = std::make_unique<systelab::web_server::Reply>();
			reply->setStatus(systelab::web_server::Reply::NOT_FOUND);
			return reply;
		}

		

		throw "Not implemented";
	}
	
}}}
