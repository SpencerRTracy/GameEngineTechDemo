#include "EventManager.h"

/**
 * Event Manager class handles manages all events and messages.
 * Any messages that need to be sent come to EventManager for
 * routing to proper destination.
 */
EventManager::EventManager(){}

/**
 * Event Manager does not need to initialize anything (rare for a component).
 */
void EventManager::Initialize(){}


/**
 * Update loop.  Routes all messages received since last update loop.
 */
void EventManager::Update(){
	Message* curMsg;
	//Empty the message list
	while(!messageList.empty()){
		curMsg = messageList.front();
		messageList.pop_front();
		if(Component* comp = ENGINE->GetComponent(curMsg->GetDestination())){
			//Send the message to its destination
			comp->ReceiveMessage(curMsg);
		} else {
			//ERROR: Destination could not be found.
			char msgbuf[256];
			sprintf(msgbuf, "%s component not found.  Unable to send message to it.", curMsg->GetDestination().c_str()); 
		}
	}
}

/**
 * Method to be called by other components for sending a message.
 */
void EventManager::SendMessage(std::string destination, int messageID, void* messageData){
	//Put the message to be sent on the messageList for routing in EventManager's update cycle.
	messageList.push_front(new Message(destination, messageID, messageData));
}

/**
 * Method to be called by other components for sending a message.
 * The message will get sent IMMEDIATELY instead of waiting for the
 * EventManager's next update cycle.
 */
void EventManager::SendMessageNow(std::string destination, int messageID, void* messageData){
	//Create Message object
	Message* curMsg = new Message(destination, messageID, messageData);

	if(Component* comp = ENGINE->GetComponent(curMsg->GetDestination())){
		//Send the message to its destination
		comp->ReceiveMessage(curMsg);
	} else {
		//ERROR: Destination could not be found.
		char msgbuf[256];
		sprintf(msgbuf, "%s component not found.  Unable to send message to it.", curMsg->GetDestination().c_str()); 
	}
}