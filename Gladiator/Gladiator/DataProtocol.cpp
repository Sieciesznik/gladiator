#include "dataprotocol.h"

MessageField::MessageField(std::string name) 
{
	this->fieldName = name;
}

std::string MessageField::getName()
{
	return this->fieldName;
}

MessageFieldInt32::MessageFieldInt32(std::string name, int32_t value)
	:MessageField(name)
{
	this->value = value;
}

int32_t MessageFieldInt32::getIntValue()
{
	return this->value;
}

double_t MessageFieldInt32::getDoubleValue()
{
	LOG_ERROR("Double value not supported for integer parameter");
	return 0;
}

std::string MessageFieldInt32::getStringValue()
{
	LOG_ERROR("String value not supported for integer parameter");
	return "";
}

MessageFieldDouble::MessageFieldDouble(std::string name, double_t value)
	:MessageField(name)
{
	this->value = value;
}

int32_t MessageFieldDouble::getIntValue()
{
	LOG_ERROR("Integer value not supported for double parameter");
	return 0;
}

double_t MessageFieldDouble::getDoubleValue()
{
	return this->value;
}

std::string MessageFieldDouble::getStringValue()
{
	LOG_ERROR("String value not supported for double parameter");
	return "";
}


MessageFieldString::MessageFieldString(std::string name, std::string value)
	:MessageField(name)
{
	this->value = value;
}

int32_t MessageFieldString::getIntValue()
{
	LOG_ERROR("Integer value not supported for string parameter");
	return 0;
}

double_t MessageFieldString::getDoubleValue()
{
	LOG_ERROR("Double value not supported for string parameter");
	return 0;
}

std::string MessageFieldString::getStringValue()
{
	return this->value;
}

MessageData::MessageData(uint8_t msgId, std::string msgName)
	: messageId(msgId), messageName(msgName) {}

MessageData::MessageData()
	: messageId(0), messageName("") {}

MessageData::~MessageData()
{
	for (std::map<std::string, MessageField *>::iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		delete it->second;
	}
}

void MessageData::addParameter(std::string fieldName, int32_t fieldVal)
{
	parameters[fieldName] = new MessageFieldInt32(fieldName, fieldVal);
}

void MessageData::addParameter(std::string fieldName, double_t fieldVal)
{
	parameters[fieldName] = new MessageFieldDouble(fieldName, fieldVal);
}

void MessageData::addParameter(std::string fieldName, std::string fieldVal)
{
	parameters[fieldName] = new MessageFieldString(fieldName, fieldVal);
}

int32_t MessageData::getIntParameter(std::string key)
{
	return parameters[key]->getIntValue();
}

double_t MessageData::getDoubleParameter(std::string key)
{
	return parameters[key]->getDoubleValue();
}

std::string MessageData::getStringParameter(std::string key)
{
	return parameters[key]->getStringValue();
}

ProtocolDecoder::ProtocolDecoder(){}

void ProtocolDecoder::set(json s2cMsgType, uint8_t size)
{
	this->serverToClientMessageTypes = s2cMsgType;
	this->messageIdSize = size;
}

MessageData ProtocolDecoder::decode(const char * byteBuffer)
{
	uint8_t msgId = byteBuffer[0];

	json messageRecipe =		this->serverToClientMessageTypes[msgId];
	json messageParameters =	messageRecipe["messageParameters"];
	MessageData newData(msgId, messageRecipe["messageName"]);

	int byteDataIterator = 1;

	for (int i = 0; i < messageParameters.size(); ++i)
	{
		if (messageParameters[i]["type"].dump() == "int")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			int32_t newValue;

			for (int j = 0; j < paramSize; ++j)
			{
				newValue |= ((int32_t)byteBuffer[byteDataIterator]) << ((paramSize - j - 1) * 8);
			}

			newData.addParameter(messageParameters[i]["name"].get<std::string>(), newValue);
			byteDataIterator += paramSize;
		}
		else if (messageParameters[i]["type"].dump() == "double")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			double_t newValue = *((double_t *)&byteBuffer[byteDataIterator]);
			
			newData.addParameter(messageParameters[i]["name"].get<std::string>(), newValue);
			byteDataIterator += paramSize;
		}
		else if (messageParameters[i]["type"].dump() == "string")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			std::string newValue;

			for (int j = 0; j < paramSize; ++j)
			{
				newValue.append(&byteBuffer[j]);
			}

			newData.addParameter(messageParameters[i]["name"].get<std::string>(), newValue);
			byteDataIterator += paramSize;
		}
	}

	return newData;
}

ProtocolEncoder::ProtocolEncoder(){}

void ProtocolEncoder::set(json c2sMsgType, uint8_t size)
{
	this->clientToServerMessageTypes = c2sMsgType;
	this->messageIdSize = size;
}
