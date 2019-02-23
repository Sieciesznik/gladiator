#include "dataprotocol.h"
#include <iostream>
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
		//delete it->second;
	}
}

void MessageData::set(uint8_t msgId, std::string msgName)
{
	this->messageId = msgId;
	this->messageName = msgName;
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
	//MessageFieldString * mfs = new MessageFieldString(fieldName, fieldVal);
	parameters[fieldName] = new MessageFieldString(fieldName, fieldVal);
	//parameters.insert(std::pair<std::string, MessageField *>(fieldName, mfs));
}

int32_t MessageData::getIntParameter(std::string key)
{
	return ((MessageFieldInt32*)parameters[key])->getIntValue();
}

double_t MessageData::getDoubleParameter(std::string key)
{
	return ((MessageFieldDouble*)parameters[key])->getDoubleValue();
}

std::string MessageData::getStringParameter(std::string key)
{
	//std::cout << parameters[key];
	return ((MessageFieldString*)parameters[key])->getStringValue();
}

uint8_t MessageData::getMsgId()
{
	return this->messageId;
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
	MessageData newData(msgId, messageRecipe["messageName"]); //= new MessageData(msgId, messageRecipe["messageName"]);

	int byteDataIterator = 1;

	for (int i = 0; i < messageParameters.size(); ++i)
	{
		if (messageParameters[i]["type"].get<std::string>() == "int")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			int32_t newValue = 0;

			if (paramSize == 1) {
				newValue = byteBuffer[byteDataIterator];
			}
			else if (paramSize == 4) {
				char tempChar[4];
				for (int j = 0; j < 4; ++j) tempChar[3 - j] = byteBuffer[byteDataIterator + j];
				memcpy(&newValue, tempChar, 4);
			}


			//for (int j = 0; j < paramSize; ++j)
			//{
		//		newValue |= ((int32_t)byteBuffer[byteDataIterator]) << ((paramSize - j - 1) * 8);
		//	}

			newData.addParameter(messageParameters[i]["name"].get<std::string>(), newValue);
			byteDataIterator += paramSize;
		}
		else if (messageParameters[i]["type"].get<std::string>() == "double")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			double_t newValue;// = *((double_t *)&byteBuffer[byteDataIterator]);

			char tempChar[8];
			for (int j = 0; j < 8; ++j) tempChar[7 - j] = byteBuffer[byteDataIterator + j];

			memcpy(&newValue, tempChar, sizeof(double_t));
			
			newData.addParameter(messageParameters[i]["name"].get<std::string>(), newValue);
			byteDataIterator += paramSize;

		}
		else if (messageParameters[i]["type"].get<std::string>() == "string")
		{
			int paramSize = (messageParameters[i]["size"].get<int>()) / 8;
			
			std::string newValue;

			newValue.append(&byteBuffer[byteDataIterator], paramSize);
			

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

std::string ProtocolEncoder::encode(MessageData msgData) 
{
	int messageSize = 1;
	json messageRecipe = this->clientToServerMessageTypes[msgData.getMsgId()];
	json messageParameters = messageRecipe["messageParameters"];

	for (int i = 0; i < messageParameters.size(); ++i)
	{
		messageSize += (messageParameters[i]["size"].get<int>()) / 8;
	}

	std::string newMsg;
//	newMsg.resize(messageSize, '\0');
	newMsg.reserve(messageSize);
	newMsg += msgData.getMsgId();
	int byteDataIterator = 1;

	for (int i = 0; i < messageParameters.size(); ++i)
	{
		int paramSize = messageParameters[i]["size"].get<int>() / 8;
		if (messageParameters[i]["type"].get<std::string>() == "int")
		{
			int32_t tempInt = msgData.getIntParameter(messageParameters[i]["name"].get<std::string>());

			for (int j = 0; j < paramSize; ++j)
			{
				newMsg += (char)(tempInt >> (j * 8));
			}
			byteDataIterator += paramSize;
		}
		else if (messageParameters[i]["type"].get<std::string>() == "double")
		{
			double_t tempDouble = msgData.getDoubleParameter(messageParameters[i]["name"].get<std::string>());

			char tempChar[8];

			memcpy(tempChar, &tempDouble, sizeof(tempDouble));

			for (int j = 0; j < 8; ++j) newMsg += tempChar[7 - j];

			byteDataIterator += paramSize;
		}
		else if (messageParameters[i]["type"].get<std::string>() == "string")
		{
			std::string tempString = msgData.getStringParameter(messageParameters[i]["name"].get<std::string>());

			newMsg.append(tempString);
			//memcpy(&newMsg[byteDataIterator], &tempString., sizeof(tempString));
			//strcpy_s(&newMsg[byteDataIterator], sizeof(tempString), tempString.c_str());
			newMsg.append(paramSize - tempString.length(), ' ');
			byteDataIterator += paramSize;
		}
	}
	return newMsg;
}