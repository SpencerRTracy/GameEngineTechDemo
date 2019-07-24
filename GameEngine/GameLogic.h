#pragma once

#include <list>
#include <vector>

extern "C" {
#include "Math2D.h"
}

#include "Component.h"
#include "MainEngine.h"
#include "ObjectCreator.h"
#include "InputManager.h"

//Forward Declaration
class GameObject;

/**
 * Component for holding all Game specific logic
 */
class GameLogic: public Component{
public:
	/**
	* Component for holding all Game specific logic
	*/
	GameLogic(int _winSizeX, int _winSizeY);
	~GameLogic(){}

	/**
	 * Initialize method for this component
	 */
	void Initialize();
	
	/**
	 * Update loop for this component
	 */
	void Update();

	/**
	 * Message has been sent from another component to this one.
	 */
	void ReceiveMessage(Message* msg);

private:
	///////////////////////////////
	//Essential Functions
	///////////////////////////////
	void Draw();
	void DebugDraw();
	void HandleMessages();
	bool IsDebugObject(GameObject* obj);
	GraphicsSystem* graphics;
	InputManager* inputManager;

	/**
	 * Draw list for graphics of all game objects to be drawn on the screen
	 */
	std::list<GameObject*> gameObjList;

	/**
	 * Mail Box.  Holds all incoming messages until it's the Game Logic Component's turn to update.
	 */
	std::list<Message*> messageList;
	
	/**
	 * EventManager used to send messages out
	 */
	EventManager* eventManager;

	//Window Sizes
	int winSizeX;
	int winSizeY;

	//Values for Debug Drawing
	bool debug;
	GameObject* debugRect;
	GameObject* debugCircle;
	GameObject* debugDirectional;
	GameObject* textDisplay;
	GameObject* smallTextDisplay;
	std::vector<GameObject*> debugCirclePieceVec;

	//Collision Type
	int collisionType;


	//GAME LOGIC FUNCTIONS BELOW

	////////////////////////////////////
	//All game object bases are declared here
	///////////////////////////////////
	GameObject* spShip;
	GameObject* background;
	GameObject* bullet;
	GameObject* missile;
	GameObject* asteroid;
	GameObject* fireAsteroid;
	GameObject* largeEnemy;
	GameObject* livesText;
	GameObject* levelText;
	GameObject* missileText;
	
	////////////////////////////////////
	//Lists
	/////////////////////////////////////
	/**
	 * List to hold all active asteroids instances
	 */
	std::list<GameObject*> asteroidList;

	/**
	 * List to hold all active bullet instances
	 */
	std::list<GameObject*> bulletList;

	/**
	 * List to hold all active missile instances
	 */
	std::list<GameObject*> missileList;


	///////////////////////////////
	//Helper variables
	///////////////////////////////
	//Frame timer vars
	float frameTime;
	int timeLastFrame;

	//Mini Boss vars
	GameObject* miniBoss;
	bool miniBossActive;

	//Rotation vars
	float rotation;
	float bgRotation;

	//Player specific vars
	int playerLives;
	int playerInvulnTimer;
	int level;

	///////////////////////////////
	//Specific Game Logic Functions
	///////////////////////////////

	/** 
	 * Handle collisions between game objects
	 */
	void HandleCollision(GameObject* obj1, GameObject* obj2);

	/**
	 * Check for input and take appropriate action
	 */
	void HandleInput();

	/**
	 * Update positions of all active game objects
	 */
	void UpdatePositions();

	/**
	 * Update game objects themselves
	 */
	void UpdateGameObjects();

	/**
	 * Check for collisions between game objects
	 */
	void CheckForCollisions();

	/** 
	 * Handle logic for changing levels
	 */
	void NextLevel();

	/**
	 * Remove an object from the active draw list
	 */
	bool RemoveFromObjList(GameObject* obj);

	/**
	 * Wrap a value around a min and max
	 */
	float WrapValue(float val, float min, float max);

	/** 
	 * Wrap an angle in radians
	 */
	float WrapDir(float angle);

};