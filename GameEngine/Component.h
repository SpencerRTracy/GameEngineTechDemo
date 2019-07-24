#pragma once

#include "Message.h"

/**
 * Component is an abstract class.
 * It is used by the MainEngine class to handle all components (Physics, GameLogic, etc.). 
 * It does not matter what class type they are.  This allows any future
 * component types to be plugged into the Engine with ease.
 */
class Component{
public:
	/**
	 * All components must have an Initialize method.
	 * This is called when the Engine is ready to begin
	 * initializing all the components registered with it.
	 */
	virtual void Initialize() = 0;
	
	/**
	 * Update is called every frame for every component (call time managed by MainEngine)
	 */
	virtual void Update(){};
	
	/**
	 * Every component registered with the engine must have a ReceiveMessage() method.
	 * This ensures that all messages/events go to where they need to.
	 * Messages do NOT have to be handled at this time.  They must just be received.
	 * Ideally the component should decide to handle messages on their Update().
	 */
	virtual void ReceiveMessage(Message* msg) {};


	/**
	 * Virtual Destructor for all components
	 */
	virtual ~Component(){}
};