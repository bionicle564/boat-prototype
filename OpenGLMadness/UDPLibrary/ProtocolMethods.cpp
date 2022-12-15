#include "ProtocolHelper.h"

//ProtocolMethods.cpp
//Gian Tullo, 0886424 / Lucas Magalhaes / Philip Tomaszewski
//23/10/21
//Provides utility methods for the protocal
//  see header for method manifest

//Protocol structure
/*
	JOIN_ROOM :  [header][len][name][len][roomName]
	LEAVE_ROOM:  [header][len][name][len][roomName]
	SEND_MSG:    [header][len][name][len][roomName][len][message]
	RECV_MSG:    [header][len][name][len][roomName][len][message]
*/

Buffer ProtocolMethods::MakeProtocol(ProtocolType type, std::string message)
{
	Buffer tempBuf(DEFAULT_BUFLEN);

	tempBuf.writeUInt32BE(message.length());
	tempBuf.writeUInt8BE(message);

	int length = tempBuf.GetWriteIndex() + 8;
	tempBuf.writeUInt32BE(0, length);

	tempBuf.writeUInt32BE(4, type);

	return tempBuf;
}

sProtocolData ProtocolMethods::ParseBuffer(Buffer input)
{
	sProtocolData data;
	if (input.GetBuffer().size() == 0) //size check
	{
		return data;
	}

	int length;
	length = input.readUInt32BE();
	data.type = (ProtocolType)input.readUInt32BE();

	uint32_t msgLength = input.readUInt32BE();
	data.message = input.readUInt8BE(msgLength);

	return data;
}