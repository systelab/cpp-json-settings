#pragma once

#include "RESTAPICore/Endpoint/IEndpoint.h"


namespace systelab { namespace setting {
	class ISettingsService;
}}

namespace systelab { namespace setting { namespace rest_api {

	class SettingsGetAllEndpoint final : public systelab::rest_api_core::IEndpoint
	{
	public:
		SettingsGetAllEndpoint(std::unique_ptr<ISettingsService>);
		virtual ~SettingsGetAllEndpoint();

		std::unique_ptr<systelab::web_server::Reply> execute(const systelab::rest_api_core::EndpointRequestData&) override;

	private:
		std::unique_ptr<ISettingsService> m_settingsService;
	};

}}}

