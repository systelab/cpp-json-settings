#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "JSONSettingsRESTAPI/SettingsGetAllEndpoint.h"

#include "JSONSettings/SettingsFile.h"
#include "JSONSettings/SettingsService.h"

#include "RapidJSONAdapter/JSONAdapter.h"
#include "RESTAPICore/Endpoint/EndpointRequestData.h"


int main(int argc, char *argv[])
{
	systelab::setting::SettingsFile settingsFile = "MySettingsFile.json";
	auto settingsService = std::make_unique<systelab::setting::SettingsService>();
	systelab::json::rapidjson::JSONAdapter jsonAdapter;
	
	auto endpoint = std::make_unique<systelab::setting::rest_api::SettingsGetAllEndpoint>
						(settingsFile, std::move(settingsService), jsonAdapter);
	endpoint->execute(systelab::rest_api_core::EndpointRequestData());

    return 0;
}
