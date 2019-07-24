#include "InputManager.h"


/**
 * Component to maintain state values of all keys.
 */
InputManager::InputManager(){}

/**
 * Nothing needs to be initialized for this Component.
 */
void InputManager::Initialize(){

}

/**
 * Message has been sent from another component to this one.
 * Message is NOT handled here.  It is only registered.
 */
void InputManager::ReceiveMessage(Message* msg){
	messageList.push_back(msg);
}


/**
 * Update loop.  Manages key state changes.
 */
void InputManager::Update(){
	//Update triggered statuses
	UpdateTriggered();

	//Handle any messages that were received
	HandleMessages();
}

/**
 * Handle the messages received since last update cycle.
 */
void InputManager::HandleMessages(){
	std::list<Message*>::iterator iter = messageList.begin();
	//Empty "Mail Box"
	while(messageList.size() != 0){
		//Handle Messages
 		Message* msg = messageList.front();
		messageList.pop_front();
		char key;
		switch (msg->Read()){
			case EventManager::KEY_DOWN:
				//Key is being held down
				key = (char) msg->GetData();
				if(!IsKeyHeld(key)){
					NewKeyTriggered(key);
				}
				NewKeyDown(key);
				break;

			case EventManager::KEY_UP:
				//Key released
				key = (char) msg->GetData();
				NewKeyUp((char) msg->GetData());
				break;
		}
		delete msg;
	}
}

/**
 * Key has been pressed or held down
 */
void InputManager::NewKeyDown(char key){
	//Key being held down
	std::unordered_map<char, bool>::iterator iter = heldMap.find(key);
	if(iter != heldMap.end()){
		iter->second = true;
	} else {
		//Key not already in map.  Needs to be added.
		heldMap.insert(std::make_pair(key, true));
	}
}

/**
 * Key has been released
 */
void InputManager::NewKeyUp(char key){
	//Key released
	std::unordered_map<char, bool>::iterator iter = heldMap.find(key);
	if(iter != heldMap.end()){
		iter->second = false;
	} else {
		//Key not already in map.  Needs to be added.
		heldMap.insert(std::make_pair(key, false));
	}
}

/**
 * Key is triggered
 */
void InputManager::NewKeyTriggered(char key){
	//Key has just been triggered
	std::unordered_map<char, bool>::iterator iter = triggeredMap.find(key);
	if(iter != triggeredMap.end()){
		iter->second = true;
	} else{
		//Key not already in map.  Needs to be added.
		triggeredMap.insert(std::make_pair(key, true));
	}

	//Add to list of keys to untrigger on next update loop
	keysToUntriggerNextUpdate.push_back(key);

}

/**
 * Update the triggered keys.
 * Remove any keys that were triggered last update cycle.
 */
void InputManager::UpdateTriggered(){
	//Update triggered status.  Untrigger previous triggered from last frame.
	std::unordered_map<char, bool>::iterator iter;
	for(unsigned int i =0; i < keysToUntriggerNextUpdate.size(); i++){
		iter = triggeredMap.find(keysToUntriggerNextUpdate[i]);
		if(iter != triggeredMap.end()){
			iter->second = false;
		} else {
			//Key not already in map.  Needs to be added.
			heldMap.insert(std::make_pair(keysToUntriggerNextUpdate[i], false));
		}
	}

	//This frame's untrigger sequence done.  Clear the list
	keysToUntriggerNextUpdate.clear();
}

/**
 * Check if the key is currently pressed.
 */
bool InputManager::IsKeyHeld(char key){
	//Look in map for the key
	std::unordered_map<char, bool>::iterator iter = heldMap.find(key);
	if(iter != heldMap.end()){
		//Key found.  Return its status
		return iter->second;
	} else{
		//Key not in map; It's never been pressed.
		return false;
	}
}

/**
 * Check if the key was just triggered.
 */
bool InputManager::IsKeyTriggered(char key){
	//Look in map for the key
	std::unordered_map<char, bool>::iterator iter = triggeredMap.find(key);
	if(iter != triggeredMap.end()){
		//Key found.  Return its status
		return iter->second;
	} else{
		//Key not in map; It's never been pressed
		return false;
	}
}