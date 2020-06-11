#include "stdafx.h"
#include "SettingsSetValueEndpoint.h"

#include "Helpers/ReplyBuilderHelper.h"
#include "Helpers/SettingCurrentValueHelper.h"
#include "Helpers/SettingLookupHelper.h"
#include "Helpers/SettingSerializationHelper.h"
#include "Helpers/SettingValueParsingHelper.h"

#include "JSONSettings/ISettingsService.h"
#include "JSONSettings/SettingDefinitionMgr.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONValue.h"

#include "RESTAPICore/Endpoint/EndpointRequestData.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace setting { namespace rest_api {

	SettingsSetValueEndpoint::SettingsSetValueEndpoint(const SettingsFile& settingsFile,
													   std::unique_ptr<ISettingsService> settingsService,
													   const systelab::json::IJSONAdapter& jsonAdapter)
		:m_settingsFile(settingsFile)
		,m_settingsService(std::move(settingsService))
		,m_jsonAdapter(jsonAdapter)
	{
	}

	SettingsSetValueEndpoint::~SettingsSetValueEndpoint() = default;

	std::unique_ptr<systelab::web_server::Reply> SettingsSetValueEndpoint::execute(const systelab::rest_api_core::EndpointRequestData& endpointRequestData)
	{
		try
		{
			if (!endpointRequestData.getParameters().hasNumericParameter("id"))
			{
				throw std::runtime_error("Configured endpoint route lacks 'id' numeric parameter.");
			}

			unsigned int settingId = endpointRequestData.getParameters().getNumericParameter("id");
			auto settingPath = SettingLookupHelper::findById(m_settingsFile, settingId);
			if (!settingPath)
			{
				return buildSettingNotFoundReply();
			}

			auto settingDefinition = SettingDefinitionMgr::get().getSetting(m_settingsFile, *settingPath);
			auto settingValueType = settingDefinition.defaultValue.type;

			std::string errorMessage;
			auto newSettingValue = SettingValueParsingHelper::parse(m_jsonAdapter, settingValueType, endpointRequestData.getContent(), errorMessage);
			if (!newSettingValue)
			{
				return buildInvalidSettingValueReply(errorMessage);
			}

			SettingCurrentValueHelper::set(*m_settingsService, m_settingsFile, *settingPath, *newSettingValue);

			auto jsonDocument = m_jsonAdapter.buildEmptyDocument();
			auto& jsonRoot = jsonDocument->getRootValue();
			jsonRoot.setType(systelab::json::OBJECT_TYPE);
			SettingSerializationHelper::serialize(*settingPath, settingDefinition, *newSettingValue, jsonRoot);

			return ReplyBuilderHelper::build(systelab::web_server::Reply::OK, jsonDocument->serialize());
		}
		catch (std::exception& exc)
		{
			return buildInternalErrorReply(exc.what());
		}
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsSetValueEndpoint::buildSettingNotFoundReply() const
	{
		return ReplyBuilderHelper::buildEmpty(systelab::web_server::Reply::NOT_FOUND);
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsSetValueEndpoint::buildInvalidSettingValueReply(const std::string& errorMessage) const
	{
		return ReplyBuilderHelper::buildMessage(systelab::web_server::Reply::BAD_REQUEST, errorMessage);
	}

	std::unique_ptr<systelab::web_server::Reply> SettingsSetValueEndpoint::buildInternalErrorReply(const std::string& errorMessage) const
	{
		return ReplyBuilderHelper::buildMessage(systelab::web_server::Reply::INTERNAL_SERVER_ERROR, errorMessage);
	}

}}}
