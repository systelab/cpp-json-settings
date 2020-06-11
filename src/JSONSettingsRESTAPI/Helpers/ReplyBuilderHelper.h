#pragma once

#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace setting { namespace rest_api {

	class ReplyBuilderHelper
	{
	public:
		static std::unique_ptr<systelab::web_server::Reply> buildEmpty(systelab::web_server::Reply::StatusType);
		static std::unique_ptr<systelab::web_server::Reply> buildMessage(systelab::web_server::Reply::StatusType, const std::string& message);
		static std::unique_ptr<systelab::web_server::Reply> build(systelab::web_server::Reply::StatusType, const std::string& content);
	};

}}}
