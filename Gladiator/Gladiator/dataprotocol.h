#pragma once

#include <stdio.h>
#include "logs.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class MessageField {

protected:
	std::string fieldName;

public:

	MessageField(std::string name);
	std::string getName();
	virtual int32_t getIntValue() = 0;
	virtual double_t getDoubleValue() = 0;
	virtual std::string getStringValue() = 0;
};

class MessageFieldInt32 : public MessageField {
	
	int32_t value;

public:
	MessageFieldInt32(std::string name, int32_t value);
	int32_t getIntValue();
	double_t getDoubleValue();
	std::string getStringValue();
};

class MessageFieldDouble : public MessageField {

	double_t value;

public:
	MessageFieldDouble(std::string name, double_t value);
	int32_t getIntValue();
	double_t getDoubleValue();
	std::string getStringValue();
};

class MessageFieldString : public MessageField {

	std::string value;

public:
	MessageFieldString(std::string name, std::string value);
	int32_t getIntValue();
	double_t getDoubleValue();
	std::string getStringValue();
};

class MessageData {

	uint8_t		messageId;
	std::string	messageName;
	std::map<std::string, MessageField*> parameters;

public:

	MessageData(uint8_t msgId, std::string messageName);
	~MessageData();
	void addParameter(std::string fieldName, int32_t fieldVal);
	void addParameter(std::string fieldName, double_t fieldVal);
	void addParameter(std::string fieldName, std::string fieldVal);
};


class ProtocolDecoder {

	uint8_t messageIdSize;
	json	serverToClientMessageTypes;

public:
	ProtocolDecoder(json s2cMsgTypes);

};

class ProtocolEncoder {

	uint8_t messageIdSize;
	json	clientToServerMessageTypes;

public:
	ProtocolEncoder(json c2sMsgTypes);

};