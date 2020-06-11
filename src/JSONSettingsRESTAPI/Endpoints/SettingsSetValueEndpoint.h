#pragma once

#include "RESTAPICore/Endpoint/IEndpoint.h"

#include "JSONSettings/SettingsFile.h"
#include "JSONSettings/SettingPath.h"
#include <boost/optional.hpp>


namespace systelab {

	namespace json {
		class IJSONAdapter;
	}
	namespace setting {
		class ISettingsService;
	}
}

namespace systelab { namespace setting { namespace rest_api {

	class SettingsSetValueEndpoint : public systelab::rest_api_core::IEndpoint
	{
	public:
		SettingsSetValueEndpoint(SettingsFile& settingsFile,
								 std::unique_ptr<ISettingsService>,
								 const systelab::json::IJSONAdapter&);
		virtual ~SettingsSetValueEndpoint();

		std::unique_ptr<systelab::web_server::Reply> execute(const systelab::rest_api_core::EndpointRequestData&) override;

	protected:
		virtual std::unique_ptr<systelab::web_server::Reply> buildSettingNotFoundReply() const;
		virtual std::unique_ptr<systelab::web_server::Reply> buildInvalidSettingValueReply(const std::string& errorMessage) const;
		virtual std::unique_ptr<systelab::web_server::Reply> buildInternalErrorReply(const std::string& errorMessage) const;

	protected:
		SettingsFile& m_settingsFile;
		std::unique_ptr<ISettingsService> m_settingsService;
		const systelab::json::IJSONAdapter& m_jsonAdapter;
	};

}}}

