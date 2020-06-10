#pragma once

#include "RESTAPICore/Endpoint/IEndpoint.h"

#include "JSONSettings/SettingsFile.h"


namespace systelab {

	namespace json {
		class IJSONAdapter;
	}
	namespace setting {
		class ISettingsService;
	}
}

namespace systelab { namespace setting { namespace rest_api {

	class SettingsGetAllEndpoint final : public systelab::rest_api_core::IEndpoint
	{
	public:
		SettingsGetAllEndpoint(const SettingsFile& settingsFile,
							   std::unique_ptr<ISettingsService>,
							   const systelab::json::IJSONAdapter&);
		virtual ~SettingsGetAllEndpoint();

		std::unique_ptr<systelab::web_server::Reply> execute(const systelab::rest_api_core::EndpointRequestData&) override;

	protected:
		virtual std::unique_ptr<systelab::web_server::Reply> buildSettingsFileNotFoundReply() const;
		virtual std::unique_ptr<systelab::web_server::Reply> buildInternalErrorReply(const std::string& errorMessage) const;

	protected:
		const SettingsFile& m_settingsFile;
		std::unique_ptr<ISettingsService> m_settingsService;
		const systelab::json::IJSONAdapter& m_jsonAdapter;
	};

}}}

