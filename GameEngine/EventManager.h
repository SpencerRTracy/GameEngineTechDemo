#pragma once

#include <list>

#include "MainEngine.h"
#include "ObjectModel.h"
#include "Message.h"


/**
 * Event Manager class handles manages all events and messages.
 * Any messages that need to be sent come to EventManager for
 * routing to proper destination.
 */
class EventManager: public Component{
	public:

		/**
		 * Enum to hold all the different message types.
		 * Can be expanded upon to hold message types for any purpose.
		 */
		enum MessageID{
			PHYSICS_CHECK,
			COLLISION,
			KEY_DOWN,
			KEY_UP,
			QUIT,
		};

		/**
		 * Event Manager class handles manages all events and messages.
		 * Any messages that need to be sent come to EventManager for
		 * routing to proper destination.
		 */
		EventManager();
		~EventManager(){}

		void Initialize();

		/**
		 * Update loop.  Routes all messages received since last update loop.
		 */
		void Update();

		/**
		 * Method to be called by other components for sending a message.
		 */
		void SendMessage(std::string destination, int messageID, void* messageData);
		
		/**
		 * Method to be called by other components for sending a message.
		 * The message will get sent IMMEDIATELY instead of waiting for the
		 * EventManager's next update cycle.
		 */
		void SendMessageNow(std::string destination, int messageID, void* messageData);


	private:
		typedef std::list<Message*> MessageList;

		/**
		 * List to hold all messages that need to be routed
		 */
		MessageList messageList;


};