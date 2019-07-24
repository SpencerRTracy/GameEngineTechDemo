#pragma once

#include <string>

//Forward Declaration to use Component class
class Component;

/**
 * Message class is a container to hold a message for the EventManager.
 */
class Message{
	public:
		/**
		 * Message class is a container to hold a message for the EventManager.
		 */
		Message(std::string _destination, int _messageID, void* _messageData);
		~Message();

		/*
		 * Returns the Message Type (KEY_UP, COLLISION, etc.)
		 */
		int Read();

		/*
		 * Returns the destination Component
		 */
		std::string GetDestination();

		/*
		 * Returns any data associated with the message.
		 */
		void* GetData();

	private:
		/**
		 * Holds the Message Type (KEY_UP, COLLISION, etc.)
		 */
		int messageID;

		/**
		 * Holds the Message Type (KEY_UP, COLLISION, etc.)
		 */
		void* messageData;

		/**
		 * Holds the destination component in a string.
		 * Does not require a type of Component or the destination
		 * for future extendability.
		 */
		std::string destination;

};