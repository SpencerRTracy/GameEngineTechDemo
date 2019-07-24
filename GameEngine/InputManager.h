#pragma once 

#include "Component.h"
#include "EventManager.h"

#define SPACE_KEY 0x20

/**
 * Component to maintain state values of all keys.
 */
class InputManager : public Component{
public:
	InputManager();

	/**
	 * Initialize method for this component
	 */
	void Initialize();

	/**
	 * Update loop.  Manages key state changes.
	 */
	void Update();

	/**
	 * Message has been sent from another component to this one.
	 * Message is NOT handled here.  It is only registered.
	 */
	void ReceiveMessage(Message* msg);

	/**
	 * Check if the key is currently pressed.
	 */
	bool IsKeyHeld(char key);

	/**
	 * Check if the key was just triggered.
	 */
	bool IsKeyTriggered(char key);
	
private:
	/**
	* List to hold all messages that have come in.
	*/
	std::list<Message*> messageList;

	/**
	 * Hash map to store any keys that become triggered
	 */
	std::unordered_map<char, bool> triggeredMap;

	/**
	 * Hash map to store any keys that are held
	 */
	std::unordered_map<char, bool> heldMap;

	/**
	 * Vector of keys that need to be untriggered on next update cycle.
	 */
	std::vector<char> keysToUntriggerNextUpdate;

	/**
	 * Handle the messages received since last update cycle.
	 */
	void HandleMessages();

	/**
	 * Key has been pressed or held down
	 */
	void NewKeyDown(char key);

	/**
	 * Key has been released
	 */
	void NewKeyUp(char key);

	/**
	 * Key is triggered
	 */
	void NewKeyTriggered(char key);

	/**
	 * Update the triggered keys.
	 * Remove any keys that were triggered last update cycle.
	 */
	void UpdateTriggered();
};