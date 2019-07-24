#include "Message.h"

/**
 * Message class is a container to hold a message for the EventManager.
 */
Message::Message(std::string _destination, int _messageID, void* _messageData)
	:	destination(_destination),
		messageID(_messageID),
		messageData(_messageData)
{

}

Message::~Message(){

}

/*
 * Returns the Message Type (KEY_UP, COLLISION, etc.)
 */
int Message::Read(){
	return messageID;
}

/*
 * Returns the destination Component
 */
std::string Message::GetDestination(){
	return destination;
}

/*
 * Returns any data associated with the message.
 * Type void* in order to pass any data type, 
 * avoiding enumeration and allowing expandibility on 
 * data types in the future.
 */
void* Message::GetData(){
	return messageData;
}