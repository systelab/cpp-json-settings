#include "stdafx.h"
#include "ReplyBuilderHelper.h"

#include <algorithm>


namespace systelab { namespace setting { namespace rest_api {

	std::unique_ptr<systelab::web_server::Reply>
	ReplyBuilderHelper::buildEmpty(systelab::web_server::Reply::StatusType status)
	{
		return build(status, "{}");
	}

	std::unique_ptr<systelab::web_server::Reply>
	ReplyBuilderHelper::buildMessage(systelab::web_server::Reply::StatusType status, const std::string& message)
	{
		std::string cleanMessage = message;
		std::replace(cleanMessage.begin(), cleanMessage.end(), '\"', '\'');
		std::replace(cleanMessage.begin(), cleanMessage.end(), '\n', ' ');
		std::replace(cleanMessage.begin(), cleanMessage.end(), '\\', '/');

		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"message\": \"" << cleanMessage << "\"" << std::endl;
		ss << "}" << std::endl;

		return build(status, ss.str());
	}

	std::unique_ptr<systelab::web_server::Reply>
	ReplyBuilderHelper::build(systelab::web_server::Reply::StatusType status, const std::string& content)
	{
		auto reply = std::make_unique<systelab::web_server::Reply>();
		reply->setStatus(status);
		reply->addHeader("Content-Type", "application/json");
		reply->setContent(content);

		return std::move(reply);
	}

}}}
