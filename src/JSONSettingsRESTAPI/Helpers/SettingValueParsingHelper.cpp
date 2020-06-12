#include "stdafx.h"
#include "SettingValueParsingHelper.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONSchemaValidator.h"
#include "JSONAdapterInterface/IJSONValue.h"

#include <boost/lexical_cast.hpp>


namespace systelab { namespace setting { namespace rest_api {

	boost::optional<SettingValue> SettingValueParsingHelper::parse(const systelab::json::IJSONAdapter& jsonAdapter,
																   const SettingValueType& valueType,
																   const std::string& requestContent,
																   std::string& errorMessage)
	{
		auto inputDocument = jsonAdapter.buildDocumentFromString(requestContent);
		if (!inputDocument)
		{
			errorMessage = "Request content not in JSON format.";
			return boost::none;
		}

		std::string schemaErrorMessage;
		if (!validateJSONSchema(jsonAdapter, *inputDocument, schemaErrorMessage))
		{
			errorMessage = "Request content does not statisfy JSON schema: " + schemaErrorMessage;
			return boost::none;
		}

		SettingValue newValue;
		newValue.type = valueType;
		newValue.value = inputDocument->getRootValue().getObjectMemberValue("newValue").getString();

		if (!validateSettingValueType(newValue))
		{
			errorMessage = "New setting value not valid.";
			return boost::none;
		}

		return newValue;
	}

	bool SettingValueParsingHelper::validateJSONSchema(const systelab::json::IJSONAdapter& jsonAdapter,
													   const systelab::json::IJSONDocument& inputDocument,
													   std::string& schemaErrorMessage)
	{
		std::stringstream ss;
		ss << "{" << std::endl;
		ss << "    \"type\": \"object\"," << std::endl;
		ss << "    \"properties\": {" << std::endl;
		ss << "         \"newValue\": { \"type\": \"string\" }" << std::endl;
		ss << "    }," << std::endl;
		ss << "    \"required\": [\"newValue\"]" << std::endl;
		ss << "}";
		std::string schemaDocumentStr = ss.str();

		auto schemaDocument = jsonAdapter.buildDocumentFromString(schemaDocumentStr);
		auto schemaValidator = jsonAdapter.buildSchemaValidator(*schemaDocument);
		
		return schemaValidator->validate(inputDocument, schemaErrorMessage);
	}

	bool SettingValueParsingHelper::validateSettingValueType(const SettingValue& newValue)
	{
		if (newValue.type == SettingValueType::BooleanValue)
		{
			if ((newValue.value != "true") && (newValue.value != "false"))
			{
				return false;
			}
		}
		else if (newValue.type == SettingValueType::IntValue)
		{
			try
			{
				boost::lexical_cast<int>(newValue.value);
			}
			catch (std::exception&)
			{
				return false;
			}
		}
		else if (newValue.type == SettingValueType::DoubleValue)
		{
			try
			{
				boost::lexical_cast<double>(newValue.value);
			}
			catch (std::exception&)
			{
				return false;
			}
		}

		return true;
	}

}}}
