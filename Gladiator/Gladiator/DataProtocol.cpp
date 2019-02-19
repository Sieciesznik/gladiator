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

ProtocolDecoder::ProtocolDecoder(json s2cMsgTypes)
{
	this->serverToClientMessageTypes = s2cMsgTypes;


}

ProtocolEncoder::ProtocolEncoder(json c2sMsgTypes)
{
	this->clientToServerMessageTypes = c2sMsgTypes;


}

