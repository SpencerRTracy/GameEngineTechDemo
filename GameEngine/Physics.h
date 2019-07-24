#pragma once

#include <list>

extern "C" {
#include "Math2D.h"
}

#include "Component.h"
#include "GameObject.h"
#include "EventManager.h"

#define PI 3.14159265f

/**
 * Component to handle all collision checking and
 * Movement responses to collision.
 */
class Physics : public Component{
public:
	/**
	 * Component to handle all collision checking and
	 * Movement responses to collision.
	 */
	Physics();
	~Physics();
	
	/**
	 * Initialize method for this component
	 */
	void Initialize();

	/**
	 * Update loop
	 */
	void Update();

	/**
	 * Update loop that needs to happen immediately.
	 * Used for collision checks.
	 */
	void UpdateNow();

	/**
	 * Message has been sent from another component to this one.
	 */
	void ReceiveMessage(Message* msg);

	/**
	 * Enum to hold all the collision volumes for objects.
	 */
	enum CollisionType{
		COLTYPE_NONE,
		COLTYPE_RECTANGLE,
		COLTYPE_CIRCLE
	};

private:
	/**
	 * EventManager used to send messages out to other components
	 */
	EventManager* eventManager;

	/**
	 * Handle the messages received since last update cycle.
	 */
	void HandleMessages();
	
	/**
	 * Mail Box.  Holds all incoming messages until it's 
	 * the Physics turn to update.
	 */
	std::list<Message*> messageList;

	/**
	 * Game object list holding all active game object instances
	 */
	std::list<GameObject*> gameObjList;

	/**
	 * Vector to hold all objects already passed over by Physics.
	 * Avoids double collision responses.
	 */
	std::vector<int> alreadyCheckedVec;

	/**
	 * Bool to test if this collision check is new.
	 */
	bool newCollisionCheck;

	/**
	 * Check if the given GameObject has already been checked by physics
	 */
	bool HasNotBeenChecked(GameObject* obj);
	
	/**
	 * Send out a collision event message.
	 * Send the two GameObjects as message data
	 */
	void SendCollisionMessage(GameObject* obj1, GameObject* obj2);
};