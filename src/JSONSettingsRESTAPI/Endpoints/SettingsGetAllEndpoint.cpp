#include "stdafx.h"
#include "SettingsGetAllEndpoint.h"

#include "Helpers/ReplyBuilderHelper.h"
#include "Helpers/SettingCurrentValueHelper.h"
#include "Helpers/SettingSerializationHelper.h"

#include "JSONSettings/ISettingsService.h"
#include "JSONSettings/SettingDefinitionMgr.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONValue.h"

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
		try
		{
			if (!SettingDefinitionMgr::get().hasFile(m_settingsFile))
			{
				return buildSettingsFileNotFoundReply();
			}

			auto jsonDocument = m_jsonAdapter.buildEmptyDocument();
			auto& jsonRoot = jsonDocument->getRootValue();
			jsonRoot.setType(systelab::json::OBJECT_TYPE);

			auto jsonSettingsArray = jsonRoot.buildValue(systelab::json::ARRAY_TYPE);
			auto settings = SettingDefinitionMgr::get().getFileSettings(m_settingsFile);
			for (const auto& settingData : settings)
			{
				auto settingPath = settingData.first;
				auto settingDefinition = settingData.second;
				auto settingCurrentValue = SettingCurrentValueHelper::query(*m_settingsService, m_settingsFile,
																			settingPath, settingDefinition.defaultValue.type);

				auto jsonSetting = jsonSettingsArray->buildValue(systelab::json::OBJECT_TYPE);
				SettingSerializationHelper::serialize(settingPath, settingDefinition, settingCurrentValue, *jsonSetting);
				jsonSettingsArray->addArrayValue(std::move(jsonSetting));
			}

			jsonRoot.addMember("settings", std::move(jsonSettingsArray));

			return ReplyBuilderHelper::build(systelab::web_server::Reply::OK, jsonDocument->serialize());
		}
		catch (std::exception& exc)
		{
			return buildInternalErrorReply(exc.what());
		}
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsGetAllEndpoint::buildSettingsFileNotFoundReply() const
	{
		return ReplyBuilderHelper::buildEmpty(systelab::web_server::Reply::NOT_FOUND);
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsGetAllEndpoint::buildInternalErrorReply(const std::string& errorMessage) const
	{
		return ReplyBuilderHelper::buildMessage(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, errorMessage);
	}

}}}
