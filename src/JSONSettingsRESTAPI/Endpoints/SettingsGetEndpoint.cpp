#include "stdafx.h"
#include "SettingsGetEndpoint.h"

#include "Helpers/ReplyBuilderHelper.h"
#include "Helpers/SettingCurrentValueHelper.h"
#include "Helpers/SettingSerializationHelper.h"

#include "JSONSettings/ISettingsService.h"
#include "JSONSettings/SettingDefinitionMgr.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONValue.h"

#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace setting { namespace rest_api {

	SettingsGetEndpoint::SettingsGetEndpoint(const SettingsFile& settingsFile,
											 std::unique_ptr<ISettingsService> settingsService,
											 const systelab::json::IJSONAdapter& jsonAdapter)
		:m_settingsFile(settingsFile)
		,m_settingsService(std::move(settingsService))
		,m_jsonAdapter(jsonAdapter)
	{
	}

	SettingsGetEndpoint::~SettingsGetEndpoint() = default;

	std::unique_ptr<systelab::web_server::Reply> SettingsGetEndpoint::execute(const systelab::rest_api_core::EndpointRequestData& endpointRequestData)
	{
		try
		{
			if (!endpointRequestData.getParameters().hasNumericParameter("id"))
			{
				throw std::runtime_error("Configured endpoint route lacks 'id' numeric parameter");
			}

			unsigned int settingId = endpointRequestData.getParameters().getNumericParameter("id");
			auto settingPath = findSetting(settingId);
			if (!settingPath)
			{
				return buildSettingNotFoundReply();
			}

			auto settingDefinition = SettingDefinitionMgr::get().getSetting(m_settingsFile, *settingPath);
			auto settingCurrentValue = SettingCurrentValueHelper::query(*m_settingsService, m_settingsFile,
																		*settingPath, settingDefinition.defaultValue.type);

			auto jsonDocument = m_jsonAdapter.buildEmptyDocument();
			auto& jsonRoot = jsonDocument->getRootValue();
			jsonRoot.setType(systelab::json::OBJECT_TYPE);
			SettingSerializationHelper::serialize(*settingPath, settingDefinition, settingCurrentValue, jsonRoot);

			return ReplyBuilderHelper::build(systelab::web_server::Reply::OK, jsonDocument->serialize());
		}
		catch (std::exception& exc)
		{
			return buildInternalErrorReply(exc.what());
		}
	}

	boost::optional<SettingPath> SettingsGetEndpoint::findSetting(int settingId) const
	{
		auto settings = SettingDefinitionMgr::get().getFileSettings(m_settingsFile);
		for (const auto& settingData : settings)
		{
			if (settingData.second.id == settingId)
			{
				return settingData.first;
			}
		}

		return boost::none;
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsGetEndpoint::buildSettingNotFoundReply() const
	{
		return ReplyBuilderHelper::buildEmpty(systelab::web_server::Reply::NOT_FOUND);
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsGetEndpoint::buildInternalErrorReply(const std::string& errorMessage) const
	{
		return ReplyBuilderHelper::buildMessage(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, errorMessage);
	}

}}}
