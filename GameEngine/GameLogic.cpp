#include "GameLogic.h"
#include "GameObject.h"

#include<time.h>

#define PI 3.14159265f

#define INITIAL_ENEMIES 10

#define INVINCIBLE_TIME				4

#define SHIP_ACCEL_FORWARD			400.0f
#define SHIP_ACCEL_BACKWARD			-600.0f
#define SHIP_ROT_SPEED				(2.0f * PI)

#define BULLET_SPEED				15.0f
#define BULLET_SIZE					5.0f		

#define INITIAL_ASTEROIDS			2
#define INITIAL_ASTEROID_SIZE		5.0
#define ASTEROID_SPEED				1.0f
#define INITIAL_ASTEROID_RADIUS		0.375f
#define ASTEROID_DEATH_THRESHOLD	2.0f
#define FIRE_AST_SPEED				4.0f

#define MINIBOSS_SPEED				5.0f

#define MISSILE_SIZE				25.0f
#define MISSILE_SPEED				4.0f
#define MISSILE_TURN_SPEED			(0.4f * PI)
#define MAX_MISSILES				3

#define SCALER					14.0f


GameLogic::GameLogic(int _winSizeX, int _winSizeY) :	
		winSizeX((int) _winSizeX * 0.8),
		winSizeY((int) _winSizeY * 0.9),
		debug(false),
		collisionType(0),
		frameTime(0.0f),
		timeLastFrame(0),
		bgRotation(0.00001f),
		rotation(0.0f),
		playerLives(5),
		playerInvulnTimer(-1),
		level(-1),
		miniBossActive(false)

{}

void GameLogic::Initialize(){
	srand(time(NULL));

	timeLastFrame = time(NULL);
	graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectCreator* objectCreator = dynamic_cast<ObjectCreator*>(ENGINE->GetComponent("ObjectCreator"));
	inputManager = dynamic_cast<InputManager*>(ENGINE->GetComponent("InputManager"));
	eventManager = dynamic_cast<EventManager*>(ENGINE->GetComponent("EventManager"));

	debugRect = objectCreator->CreateGameObject("GameObjects/debugRect.txt");
	debugCircle = objectCreator->CreateGameObject("GameObjects/debugCircle.txt");
	debugDirectional = objectCreator->CreateGameObject("GameObjects/debugCirclePiece.txt");

	//GAME LOGIC INITIALIZATION

	// Load Game Objects that become active at a later time
	// Serializes objects at load-time instead of runtime.
	// Objects info can be copied to create instances.
	bullet = objectCreator->CreateGameObject("GameObjects/bullet.txt");
	missile = objectCreator->CreateGameObject("GameObjects/missile.txt");
	asteroid = objectCreator->CreateGameObject("GameObjects/asteroid.txt");
	asteroid->SetSpeed(ASTEROID_SPEED);
	fireAsteroid = objectCreator->CreateGameObject("GameObjects/fireasteroid.txt");
	fireAsteroid->SetMultiplied(true);
	fireAsteroid->SetSpeed(FIRE_AST_SPEED);
	largeEnemy = objectCreator->CreateGameObject("GameObjects/enemy.txt");
	largeEnemy->SetSpeed(MINIBOSS_SPEED);
	largeEnemy->SetRotX(0.05f);
	largeEnemy->SetRotY(0.1f);

	//Load immediately active game objects
	spShip = objectCreator->CreateGameObject("GameObjects/player.txt");
	spShip->SetCurrentDir(0.0f);

	//Game object that holds the Background Textured Quad
	background = objectCreator->CreateGameObject("GameObjects/background.txt");
	
	//Game Object that only displays text to the screen
	textDisplay = objectCreator->CreateGameObject("GameObjects/TextDisplay.txt");
	smallTextDisplay = new GameObject(*textDisplay);
	smallTextDisplay->SetModelName("SmallTextDisplay");

	NextLevel();
	
}

/**
 * Message has been sent from another component to this one.
 * Message is NOT handled here.  It is only registered.
 */
void GameLogic::ReceiveMessage(Message* msg){
	messageList.push_back(msg);
}

/**
 * Handle all messages that were received
 */
void GameLogic::HandleMessages(){
	std::list<Message*>::iterator iter = messageList.begin();
	//Empty "Mail Box"
	while(messageList.size() != 0){
		//Handle Messages
 		Message* msg = messageList.front();
		messageList.pop_front();
		switch (msg->Read()){
			case EventManager::COLLISION:
				HandleCollision( ((std::pair<GameObject*, GameObject*>*) msg->GetData())->first, ((std::pair<GameObject*, GameObject*>*) msg->GetData())->second);	
				delete msg->GetData();
				break;
		}
		delete msg;
	}
}

/**
 * Update loop for this component
 */
void GameLogic::Update(){
	//Handle all messages from the "Mail Box" (aka messageList)
	HandleMessages();

	//Clear dummy debug objects
	if(debug){
		std::list<GameObject*>::iterator iter = gameObjList.begin();
		while(iter != gameObjList.end()){
			if(IsDebugObject(*iter)){
				iter = gameObjList.erase(iter);
			} else{
				++iter;
			}
		}
	}

	//ALL MAIN GAME LOOP LOGIC GOES HERE
	
	//Handle Special Level Cases (Tutorial, Game Over, etc.)
	if(level <= 0){
		if(inputManager->IsKeyTriggered(SPACE_KEY)){
			if(level == -2){
				++level;
			}
			NextLevel();
		}
		Draw();
	} else{

		// ======================
		// Getting the frame time
		// ======================

		//Game locked to 60fps
		frameTime = 1/60.0f;

		// =========================
		// Update according to input
		// =========================

		HandleInput();

		// ==================================================
		// Update positions of all active game object instances
		// ==================================================
	
		UpdatePositions();

		// ===================================
		// Update active game object instances
		// Example:
		//		-- Wrap specific object instances around the world (Needed for the assignment)
		//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
		//		-- If you have a homing missile for example, compute its new orientation (Homing missiles are not required for the Asteroids project)
		//		-- Update a particle effect (Not required for the Asteroids project)
		// ===================================
	
		UpdateGameObjects();

		// ====================================================
		// Send active game objects to Physics Component
		// ====================================================
	
		//Send game objects to physics for testing using event passing
		CheckForCollisions();

		//Handle the Physics results
		HandleMessages();

		// ==================================
		// Prepare Game Objects for Drawing
		// ==================================
	
		Draw();
	}
}

/**
 * Check for input and take appropriate action
 */
void GameLogic::HandleInput(){
	//Skip level
	if(inputManager->IsKeyTriggered('P')){
		NextLevel();
	}

	//Activate live debug drawing
	if(inputManager->IsKeyTriggered('U')){
		debug = !debug;
	}
	
	//Check if forward thrusters active
	if (inputManager->IsKeyHeld('W')){
		Vector2D unitDirVec;
		Vector2D accelVec;
		float newXVel;
		float newYVel;

		//Computing Acceleration Vector
		Vector2DFromAngleRad(&unitDirVec, spShip->GetCurrentDir());
		Vector2DScale(&accelVec, &unitDirVec, SHIP_ACCEL_FORWARD);

		newXVel = accelVec.x * frameTime + spShip->GetXVelocity();
		newYVel = accelVec.y * frameTime + spShip->GetYVelocity();
		
		newXVel = newXVel * 0.99f;
		newYVel = newYVel * 0.99f;	

		spShip->SetXVelocity(newXVel);
		spShip->SetYVelocity(newYVel);
	}

	//Check if reverse thrusters active
	if (inputManager->IsKeyHeld('S')){
		Vector2D unitDirVec;
		Vector2D accelVec;
		float newXVel;
		float newYVel;

		//Computing Acceleration Vector
		Vector2DFromAngleRad(&unitDirVec, spShip->GetCurrentDir());
		Vector2DScale(&accelVec, &unitDirVec, SHIP_ACCEL_BACKWARD);

		newXVel = accelVec.x * frameTime + spShip->GetXVelocity();
		newYVel = accelVec.y * frameTime + spShip->GetYVelocity();
		
		newXVel = newXVel * 0.99f;
		newYVel = newYVel * 0.99f;	

		spShip->SetXVelocity(newXVel);
		spShip->SetYVelocity(newYVel);
		
	}

	//Check if rotate left key
	if (inputManager->IsKeyHeld('A')){
		spShip->SetCurrentDir(spShip->GetCurrentDir() + SHIP_ROT_SPEED * (float)(frameTime));
		spShip->SetCurrentDir(WrapValue(spShip->GetCurrentDir(), -PI, PI));
	}

	//Check if rotate right key
	if (inputManager->IsKeyHeld('D')){
		spShip->SetCurrentDir(spShip->GetCurrentDir() - SHIP_ROT_SPEED * (float)(frameTime));
		spShip->SetCurrentDir(WrapValue(spShip->GetCurrentDir(), -PI, PI));
	}

	// Shoot a bullet if Space Bar is triggered (Create a new object instance)
	if (inputManager->IsKeyTriggered(SPACE_KEY)){
		Vector2D bulletVelocity = {BULLET_SPEED, BULLET_SPEED};

		//Create Game Object Instance of Bullet
		GameObject* newBullet = new GameObject(*bullet);

		//Use Player Ship's position as starting position for Bullet
		newBullet->SetXPos(spShip->GetXPos());
		newBullet->SetYPos(spShip->GetYPos());
		
		//Use Player Ship's velocity as a base velocity for Bullet
		newBullet->SetXVelocity(spShip->GetXVelocity());
		newBullet->SetYVelocity(spShip->GetYVelocity());

		//Use Player Ship's direction for Bullet to get it to fire in the proper direction
		newBullet->SetCurrentDir(spShip->GetCurrentDir());
		newBullet->SetRotZ(spShip->GetRotZ());

		//Add bullet to the bullet list for updates and management
		bulletList.push_front(newBullet);

		//Add bullet list to the draw list
		gameObjList.push_back(newBullet);
	}

	// Shoot a homing missle if 'M' is triggered (Create a new object instance)
	if (inputManager->IsKeyTriggered('F')){
		if(missileList.size() < MAX_MISSILES){
			//Create Game Object Instance of Missile
			GameObject* newMissile = new GameObject(*missile);

			//Use Player Ship as a starting point for Missile
			newMissile->SetXPos(spShip->GetXPos());
			newMissile->SetYPos(spShip->GetYPos());

			//Set Missile speed
			newMissile->SetXVelocity(MISSILE_SPEED);
			newMissile->SetYVelocity(MISSILE_SPEED);

			//Use Player Ship's direction as an initial direction for the missile
			newMissile->SetCurrentDir(spShip->GetCurrentDir());

			//Add missile to the missile list
			missileList.push_front(newMissile);

			//Add missile to the draw list
			gameObjList.push_back(newMissile);
		}
	}
}

/**
 * Update positions of all active game objects
 */
void GameLogic::UpdatePositions(){
	//Update Ship Position
	spShip->SetXPos(spShip->GetXPos() + spShip->GetXVelocity() * frameTime);
	spShip->SetYPos(spShip->GetYPos() + spShip->GetYVelocity() * frameTime);

	//Update MiniBoss if active
	if(miniBossActive){
		Vector2D unitVector;
		Vector2D scaledVector;
		Vector2DFromAngleRad(&unitVector, miniBoss->GetCurrentDir());
		Vector2DScale(&scaledVector, &unitVector, miniBoss->GetSpeed());

		Vector2D newPosition;
		Vector2D oldPosition = {miniBoss->GetXPos(), miniBoss->GetYPos()};
		Vector2DAdd(&newPosition, &oldPosition, &scaledVector);
		miniBoss->SetXPos(newPosition.x);
		miniBoss->SetYPos(newPosition.y);
	}
	
	//Iterator for traversing various GameObject lists
	std::list<GameObject*>::iterator iter;

	//Update Bullet Positions
	for (iter = bulletList.begin(); iter != bulletList.end(); ++iter){
		Vector2D unitVector;
		Vector2D scaledVector;
		Vector2DFromAngleRad(&unitVector, (*iter)->GetCurrentDir());
		Vector2DScale(&scaledVector, &unitVector, BULLET_SPEED);

		Vector2D newPosition;
		Vector2D oldPosition = {(*iter)->GetXPos(), (*iter)->GetYPos()};
		Vector2DAdd(&newPosition, &oldPosition, &scaledVector);
		(*iter)->SetXPos(newPosition.x);
		(*iter)->SetYPos(newPosition.y);
	}

	//Update Asteroid Positions
	for (iter = asteroidList.begin(); iter != asteroidList.end(); ++iter){
		Vector2D unitVector;
		Vector2D scaledVector;
		Vector2DFromAngleRad(&unitVector, (*iter)->GetCurrentDir());
		Vector2DScale(&scaledVector, &unitVector, (*iter)->GetSpeed());

		Vector2D newPosition;
		Vector2D oldPosition = {(*iter)->GetXPos(), (*iter)->GetYPos()};
		Vector2DAdd(&newPosition, &oldPosition, &scaledVector);
		(*iter)->SetXPos(newPosition.x);
		(*iter)->SetYPos(newPosition.y);
	}
}

/**
 * Update game objects themselves
 */
void GameLogic::UpdateGameObjects(){

	//Update UI
	char text[100];
	sprintf(text, "Lives: %i", playerLives);
	livesText->SetText(text);

	sprintf(text, "Level: %i", level);
	levelText->SetText(text);

	sprintf(text, "%i / %i", missileList.size(), MAX_MISSILES);
	missileText->SetText(text);
	float xPos = spShip->GetXPos();
	missileText->SetXPos(xPos + xPos/2 -90.0f);
	float yPos = spShip->GetYPos();
	missileText->SetYPos(yPos + yPos*0.2f);

	//Update Player Object
	if(time(NULL) - playerInvulnTimer > INVINCIBLE_TIME){
		//Player not momentarily invulnerable from a previous death
		spShip->SetCollisionType(1);
		spShip->SetModelName("Ship");
	} else{
		spShip->SetCollisionType(0);
		spShip->SetModelName("ShipHit");
	}
	
	//Warp the ship from one end of the screen to the other
	spShip->SetXPos(WrapValue(spShip->GetXPos(), winSizeX/-2.0f, winSizeX/2.0f));
	spShip->SetYPos(WrapValue(spShip->GetYPos(), winSizeY/-2.0f, winSizeY/2.0f));
	
	//Make ship face proper direction
	spShip->SetRotZ(spShip->GetCurrentDir());

	//Update Pyramid Enemy
	if(!miniBossActive && rand()%(400/level) == 1){
		miniBoss = new GameObject(*largeEnemy);
		miniBoss->SetName("MiniBoss");
		miniBoss->SetCurrentDir( (rand()%360) * PI/180.0f);
		gameObjList.push_back(miniBoss);
		miniBossActive = true;
	}

	if(miniBossActive){
		miniBoss->SetXPos(WrapValue(miniBoss->GetXPos(), winSizeX/-2.0f, winSizeX/2.0f));
		miniBoss->SetYPos(WrapValue(miniBoss->GetYPos(), winSizeY/-2.0f, winSizeY/2.0f));
		miniBoss->SetRotX(miniBoss->GetRotX() + 0.000f);
		miniBoss->SetRotY(miniBoss->GetRotY() + 0.075f);
	}
	
	//Iterator for traversing various GameObject lists
	std::list<GameObject*>::iterator iter;

	//Update all Asteroids
	for(iter = asteroidList.begin(); iter != asteroidList.end(); ++iter){
		//Warp the asteroids from one end of the screen to the other
		(*iter)->SetXPos(WrapValue((*iter)->GetXPos(), winSizeX/-2 - (((*iter)->GetScaleX() + (*iter)->GetScaleY())/2) * INITIAL_ASTEROID_RADIUS, winSizeX/2 + (((*iter)->GetScaleX() + (*iter)->GetScaleY())/2) * INITIAL_ASTEROID_RADIUS));
		(*iter)->SetYPos(WrapValue((*iter)->GetYPos(), winSizeY/-2 - (((*iter)->GetScaleX() + (*iter)->GetScaleY())/2) * INITIAL_ASTEROID_RADIUS, winSizeY/2 + (((*iter)->GetScaleX() + (*iter)->GetScaleY())/2) * INITIAL_ASTEROID_RADIUS));
	}

	//Update all Bullets
	iter = bulletList.begin();
	while(iter != bulletList.end()){
		// Remove bullets that go out of bounds
		Vector2D origin = {0.0f, 0.0f};
		Vector2D posCurr = {(*iter)->GetXPos(), (*iter)->GetYPos()};
		if(!StaticRectToStaticRect(&origin, winSizeX, winSizeY, &posCurr, BULLET_SIZE, BULLET_SIZE)){
			//Destroying Bullet (Bullet Reached Edge of Screen)
			
			RemoveFromObjList(*iter);

			delete (*iter);
			iter = bulletList.erase(iter);
		} else{
			//Make bullet face proper direction
			(*iter)->SetRotZ((*iter)->GetCurrentDir());
			++iter;
		}


	}
	
	// Update missile (Check if previous target is still alive, ajudst angle, find new target etc..)
	for (iter = missileList.begin(); iter != missileList.end(); ++iter){
		//Pointer for holding a target for the homing missile to follow
		GameObject* targetAst;
		//Variables for calculating angle to asteroid
		float squareDistToTargetAst = (float) ((winSizeX) * (winSizeX));
		Vector2D unitVector;
		Vector2D scaledVector;

		//Iterator for traversing Asteroid object list
		std::list<GameObject*>::iterator astIter;
		
		for(astIter = asteroidList.begin(); astIter != asteroidList.end(); ++astIter){
			//Find closest Asteroid
			float testDistance;
			Vector2D missilePos = {(*iter)->GetXPos(), (*iter)->GetYPos()};
			Vector2D asteroidPos = {(*astIter)->GetXPos(), (*astIter)->GetYPos()};
			testDistance = Vector2DSquareDistance(&missilePos, &asteroidPos);
			if(testDistance < squareDistToTargetAst){
				targetAst = *astIter;
				squareDistToTargetAst = testDistance;
			}
		}

		if(squareDistToTargetAst >= (winSizeX)*(winSizeX)){
			//Error finding an asteroid target for missile
		} else{
			float crossProd;
			Vector2D vecToAst;
			Vector2D missilePos = {(*iter)->GetXPos(), (*iter)->GetYPos()};
			Vector2D asteroidPos = {targetAst->GetXPos(), targetAst->GetYPos()};
			Vector2DSub(&vecToAst, &asteroidPos, &missilePos);
			Vector2DFromAngleRad(&unitVector, (*iter)->GetCurrentDir());

			crossProd = vecToAst.x * unitVector.y - vecToAst.y * unitVector.x;
				
			if(crossProd < 0){
				(*iter)->SetCurrentDir((*iter)->GetCurrentDir() + MISSILE_TURN_SPEED * (float)frameTime);
			} else {
				(*iter)->SetCurrentDir((*iter)->GetCurrentDir() - MISSILE_TURN_SPEED * (float)frameTime);
			}

			Vector2DFromAngleRad(&unitVector, (*iter)->GetCurrentDir());
			Vector2DScale(&scaledVector, &unitVector, MISSILE_SPEED);
			Vector2D finalMissilePos;
			Vector2DAdd(&finalMissilePos, &missilePos, &scaledVector);
			(*iter)->SetXPos(finalMissilePos.x);
			(*iter)->SetYPos(finalMissilePos.y);
		}

		//Make missile face proper direction
		(*iter)->SetRotZ((*iter)->GetCurrentDir());

	}

	if(asteroidList.size() <= 0){
		//Next Level
		NextLevel();
	}

	//Update Background
	background->SetRotZ(bgRotation);
	bgRotation += 0.0002f * level;


	//Update frame time
	timeLastFrame = time(NULL);
}

/**
 * Check for collisions between game objects
 */
void GameLogic::CheckForCollisions(){
	eventManager->SendMessageNow("Physics", eventManager->PHYSICS_CHECK, &gameObjList);
}

/** 
 * Handle collisions between game objects
 */
void GameLogic::HandleCollision(GameObject* obj1, GameObject* obj2){
	if(obj1->GetGUID() < 0 || obj1->GetGUID() > GameObject::instanceCounter || obj2->GetGUID() < 0 || obj2->GetGUID() > GameObject::instanceCounter){
		char msgbuf[256];
		sprintf(msgbuf, "Invalid Pointer\n"); 
		OutputDebugString(msgbuf);
	}
	// check if the object is an Asteroid
	else if (obj1->GetName() == "Asteroid" || obj1->GetName() == "FireAsteroid"){
		//Check if the other object is another Asteroid
		if(obj2->GetName() == "Asteroid" && obj1->GetName() != "FireAsteroid"){
			if(!obj1->HasMultiplied() || !obj2->HasMultiplied()){
				obj1->SetMultiplied(true);
				obj2->SetMultiplied(true);
				
				GameObject* newAst = new GameObject(*fireAsteroid);
				newAst->SetCurrentDir( (rand() % 360) * PI/180.0f);
				newAst->SetXPos((obj1->GetXPos() + obj2->GetXPos())/2);
				newAst->SetYPos((obj1->GetYPos() + obj2->GetYPos())/2);
				asteroidList.push_back(newAst);
				gameObjList.push_back(newAst) ;
				
			}
		}

		// check if the object is the Player Ship
		else if (obj2->GetName() == "Player"){
			//Ship and Asteroid Collision
			playerInvulnTimer = time(NULL);
			if(playerLives > 0){
				GameObject* newAst = new GameObject(*fireAsteroid);
				newAst->SetCurrentDir( (rand() % 360) * PI/180.0f);
				newAst->SetXPos((obj1->GetXPos() + obj2->GetXPos())/2);
				newAst->SetYPos((obj1->GetYPos() + obj2->GetYPos())/2);
				newAst->SetScaleX(obj1->GetScaleX());
				newAst->SetScaleY(obj1->GetScaleY());
				newAst->SetSpeed(obj1->GetSpeed() *2);
				asteroidList.push_back(newAst);
				gameObjList.push_back(newAst) ;

				playerLives--;
				obj2->SetXPos(0);
				obj2->SetYPos(0);
				obj2->SetXVelocity(0);
				obj2->SetYVelocity(0);
				obj2->SetCurrentDir(0);
			} else{
				//Game Over
				level = -2;
				playerLives = 5;
				NextLevel();
			}
		} else if (obj2->GetName() == "Bullet"){
			//Bullet and Asteroid Collision
			
			//Remove bullet from active object list
			RemoveFromObjList(obj2);
			
			//Destroy bullet
			std::list<GameObject*>::iterator iter = bulletList.begin();
			while(iter != bulletList.end()){
				if(obj2->GetGUID() == (*iter)->GetGUID()){
					delete (*iter);
					iter = bulletList.erase(iter);
					break;
				}
				++iter;
			}

			//New Asteroid Scale
			if( (obj1->GetScaleX() + obj1->GetScaleY()) / 2 < ASTEROID_DEATH_THRESHOLD){
				//Asteroid too small - Destroy it
				RemoveFromObjList(obj1);

				std::list<GameObject*>::iterator iter = asteroidList.begin();
				while(iter != asteroidList.end()){
					if(obj1->GetGUID() == (*iter)->GetGUID()){
						delete (*iter);
						iter = asteroidList.erase(iter);
						break;
					}
					++iter;
				}
			} else{
				obj1->SetScaleX(obj1->GetScaleX() * 0.75f);
				obj1->SetScaleY(obj1->GetScaleY() * 0.75f);
			}
		} else if(obj2->GetName() == "Missile"){
			//Missile and Asteroid Collision
			
			//Remove missile from active object list
			RemoveFromObjList(obj2);
			
			//Destroy missile
			std::list<GameObject*>::iterator iter = missileList.begin();
			while(iter != missileList.end()){
				if(obj2->GetGUID() == (*iter)->GetGUID()){
					delete (*iter);
					iter = missileList.erase(iter);
					break;
				}
				++iter;
			}

			//New Asteroid Scale
			if((obj1->GetScaleX() + obj1->GetScaleY()) / 2 < ASTEROID_DEATH_THRESHOLD){
				//Asteroid too small - Destroy it
				RemoveFromObjList(obj1);

				std::list<GameObject*>::iterator iter = asteroidList.begin();
				while(iter != asteroidList.end()){
					if(obj1->GetGUID() == (*iter)->GetGUID()){
						delete (*iter);
						iter = asteroidList.erase(iter);
						break;
					}
					++iter;
				}
			} else{
				obj1->SetScaleX(obj1->GetScaleX() * 0.5f);
				obj1->SetScaleY(obj1->GetScaleY() * 0.5f);
			}
		}
	} else if (obj1->GetName() == "Bullet"){
		//Bullet and Fire Asteroid Collision
		if(obj2->GetName() == "FireAsteroid"){
			//Remove bullet from active object list
			RemoveFromObjList(obj1);
			
			//Destroy bullet
			std::list<GameObject*>::iterator iter = bulletList.begin();
			while(iter != bulletList.end()){
				if(obj1->GetGUID() == (*iter)->GetGUID()){
					delete (*iter);
					iter = bulletList.erase(iter);
					break;
				}
				++iter;
			}

			//New Asteroid Scale
			if((obj2->GetScaleX() + obj2->GetScaleY()) / 2 < ASTEROID_DEATH_THRESHOLD){
				//Asteroid too small - Destroy it
				RemoveFromObjList(obj2);

				std::list<GameObject*>::iterator iter = asteroidList.begin();
				while(iter != asteroidList.end()){
					if(obj2->GetGUID() == (*iter)->GetGUID()){
						delete (*iter);
						iter = asteroidList.erase(iter);
						break;
					}
					++iter;
				}
			} else{
				obj2->SetScaleX(obj2->GetScaleX() * 0.75f);
				obj2->SetScaleY(obj2->GetScaleY() * 0.75f);
			}
		}

		else if(obj2->GetName() == "MiniBoss"){
			//Remove bullet from active object list
			RemoveFromObjList(obj1);
			
			//Destroy bullet
			std::list<GameObject*>::iterator iter = bulletList.begin();
			while(iter != bulletList.end()){
				if(obj1->GetGUID() == (*iter)->GetGUID()){
					delete (*iter);
					iter = bulletList.erase(iter);
					break;
				}
				++iter;
			}

			//Mini Boss hit
			RemoveFromObjList(obj2);
			miniBossActive = false;
			delete miniBoss;
		}
	}

	else if(obj1->GetName() == "MiniBoss"){
		// check if the object is the Player Ship
		if (obj2->GetName() == "Player"){
			//Ship and Asteroid Collision
			playerInvulnTimer = time(NULL);
			if(playerLives > 0){
				playerLives--;
				obj2->SetXPos(0);
				obj2->SetYPos(0);
				obj2->SetXVelocity(0);
				obj2->SetYVelocity(0);
				obj2->SetCurrentDir(0);
			} else{
				//Game Over
				level = -2;
				playerLives = 5;
				NextLevel();
			}
		} else if(obj2->GetName() == "Missile"){
			//Missile and Asteroid Collision
			
			//Remove missile from active object list
			RemoveFromObjList(obj2);
			
			//Destroy missile
			std::list<GameObject*>::iterator iter = missileList.begin();
			while(iter != missileList.end()){
				if(obj2->GetGUID() == (*iter)->GetGUID()){
					delete (*iter);
					iter = missileList.erase(iter);
					break;
				}
				++iter;
			}

			//Mini Boss hit
			RemoveFromObjList(obj1);
			miniBossActive = false;
			delete miniBoss;
		}
	}


}

/**
 * Update graphics system with list of objects to draw this frame
 */
void GameLogic::Draw(){
	if(debug)
		DebugDraw();
	else
		//Graphics called directly to avoid delays with Message Passing
		//It will NOT draw until the Graphics component's turn to update, following the component-based style
		//It simply updates the game object list of objects to draw
		//Future features such as message load balancing can interfere
		//Better performance to make an exception for graphics to have a "Direct Line"
		graphics->UpdateGameObjects(&gameObjList);
}

/**
 * Ontop of normal draw, draw debug objects
 */
void GameLogic::DebugDraw(){
	int circleIndex = 0;
	for(std::list<GameObject*>::iterator iter = gameObjList.begin(); iter != gameObjList.end(); ++iter){
		//Set debug movement indicators
		if((*iter)->GetCollisionType() != 0 || (*iter)->GetName() == "Player"){
			GameObject* debugObj = new GameObject(*(*iter));
			debugObj->SetName("DebugDirectional");
			debugObj->SetCollisionType(debugDirectional->GetCollisionType());
			debugObj->SetLayer((*iter)->GetLayer() - 0.1f);
			debugObj->SetModelName(debugDirectional->GetModelName());
			debugObj->SetScaleY(debugDirectional->GetScaleY());
			if((*iter)->GetName() == "Player"){
				float xVel = debugObj->GetXVelocity();
				float yVel = debugObj->GetYVelocity();
				float angle = atan(yVel/xVel);
				if(xVel > 0){
					//angle = angle;
				} else if(xVel < 0){
					angle -= PI;
				} else if(xVel == 0){
					if(yVel > 0){
						angle = PI;
					} else if(yVel < 0){
						angle = -PI;
					}
				}

				debugObj->SetScaleX(sqrt(xVel*xVel + yVel*yVel) / 50.0f);
				debugObj->SetRotZ(angle);
			}
			else if((*iter)->GetName().find("Asteroid") != std::string::npos){
				debugObj->SetScaleX(debugObj->GetSpeed() / 1.0f);
				debugObj->SetRotZ(debugObj->GetCurrentDir());
			}
				
			gameObjList.push_back(debugObj);


			//Create Position Text
			GameObject* obj;
			char text[20];
			sprintf(text, "(%3.2f, %3.2f)", (*iter)->GetXPos(), (*iter)->GetYPos());
			obj = new GameObject(*smallTextDisplay);
			obj->SetName("DebugText");
			obj->SetText(text);
			float xPos = (*iter)->GetXPos();
			obj->SetXPos(xPos + xPos/2.0f + 50.0f);
			float yPos = (*iter)->GetYPos();
			obj->SetYPos(yPos + yPos*0.2f);
			obj->SetLayer((*iter)->GetLayer() - 0.1f);
			gameObjList.push_back(obj);


		}

		//Set the collision boxes for display
		if((*iter)->GetCollisionType() == 1){
			//Rectangle collision area needs to be displayed
			GameObject* debugObj = new GameObject(*(*iter));
			debugObj->SetName("DebugRect");
			debugObj->SetCollisionType(debugRect->GetCollisionType());
			debugObj->SetLayer((*iter)->GetLayer() + 0.1f);
			debugObj->SetModelName(debugRect->GetModelName());
			debugObj->SetScaleY(debugObj->GetScaleX() * 0.8f);
			debugObj->SetRotY(0.0f);
			gameObjList.push_back(debugObj);
		} else if((*iter)->GetCollisionType() == 2){
			//Circle collision area needs to be displayed
			GameObject* debugObj = new GameObject(*(*iter));
			debugObj->SetName("DebugCircle");
			debugObj->SetCollisionType(debugCircle->GetCollisionType());
			debugObj->SetLayer((*iter)->GetLayer() + 0.1f);
			debugObj->SetModelName(debugCircle->GetModelName());
			debugObj->SetScaleX(debugObj->GetScaleX());
			debugObj->SetScaleY(debugObj->GetScaleY());
			debugObj->SetRotY(0.0f);
			gameObjList.push_back(debugObj);	
		}
	}
	graphics->UpdateGameObjects(&gameObjList);
}

/** 
 * Handle logic for changing levels
 */
void GameLogic::NextLevel(){
	if(level >= 5){
		level = -3;
	} else{
		++level;
	}

	//Reset all lists
	asteroidList.clear();
	bulletList.clear();
	missileList.clear();
	gameObjList.clear();
	debugCirclePieceVec.clear();
	if(miniBossActive){
		miniBossActive = false;
		delete miniBoss;
	}

	if(level > 0){
		char text[100];
		sprintf(text, "Lives: %i", playerLives);
		livesText = new GameObject(*textDisplay);
		livesText->SetName("UIText");
		livesText->SetText(text);
		livesText->SetXPos(winSizeX/2.0f - 100.0f);
		livesText->SetYPos(winSizeY/2.0f + 20.0f);
		gameObjList.push_back(livesText);

		sprintf(text, "Level: %i", level);
		levelText = new GameObject(*textDisplay);
		levelText->SetName("UIText");
		levelText->SetText(text);
		levelText->SetXPos(-100.0f);
		levelText->SetYPos(winSizeY/2.0f + 20.0f);
		gameObjList.push_back(levelText);

		sprintf(text, "%i / %i", missileList.size(), MAX_MISSILES);
		missileText = new GameObject(*smallTextDisplay);
		missileText->SetName("UIText");
		missileText->SetText(text);
		missileText->SetXPos(0.0f);
		missileText->SetYPos(0.0f);
		gameObjList.push_back(missileText);

		gameObjList.push_back(background);
		gameObjList.push_back(spShip);

		int offsetX[] = {0,  0,  1,  1,  1, -1, -1, -1};
		int offsetY[] = {1, -1,  0,  1, -1,  0,  1, -1};

		int currentAsteroids = INITIAL_ASTEROIDS + level/2;

		for(int i = 0; i < currentAsteroids; i++){
			GameObject* newAst = new GameObject(*asteroid);
			newAst->SetSpeed(newAst->GetSpeed() * level);
			newAst->SetCurrentDir((rand() % 360) * PI/180.0f);
			//newAst->SetXPos(7.0f * newAst->GetScaleX()/2);
			newAst->SetXPos(350.0f * offsetX[i%8]);
			newAst->SetYPos(350.0f * offsetY[i%8]);
			asteroidList.push_back(newAst);
			gameObjList.push_back(newAst);
		}
	} else if(level == 0) {
		//Tutorial Level
		GameObject* obj;
		
		obj = new GameObject(*textDisplay);
		obj->SetText("Asteroids");
		obj->SetXPos(-90.0f);
		obj->SetYPos(350.0f);
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetText("Goal is to clear the screen of all objects");
		obj->SetXPos(-375.0f);
		obj->SetYPos(200.0f);
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-300.0f);
		obj->SetYPos(0.0f);
		obj->SetText("W, A, S, D        -         Move Ship");
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-300.0f);
		obj->SetYPos(-50.0f);
		obj->SetText("SPACE             -         Shoot");
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-300.0f);
		obj->SetYPos(-100.0f);
		obj->SetText("F                       -         Fire Missile");
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-230.0f);
		obj->SetYPos(-300.0f);
		obj->SetText("Press SPACE to Continue");
		gameObjList.push_back(obj);
	} else if(level == -2){
		//Victory Screen
		GameObject* obj;
		
		obj = new GameObject(*textDisplay);
		obj->SetText("Victory");
		obj->SetXPos(-80.0f);
		obj->SetYPos(300.0f);
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-230.0f);
		obj->SetYPos(-300.0f);
		obj->SetText("Press SPACE to Continue");
		gameObjList.push_back(obj);
	} else {
		//Game Over screen
		GameObject* obj;
		
		obj = new GameObject(*textDisplay);
		obj->SetText("Game Over");
		obj->SetXPos(-90.0f);
		obj->SetYPos(300.0f);
		gameObjList.push_back(obj);

		obj = new GameObject(*textDisplay);
		obj->SetXPos(-230.0f);
		obj->SetYPos(-300.0f);
		obj->SetText("Press SPACE to Continue");
		gameObjList.push_back(obj);
	}
}

/**
 * Remove an object from the active draw list
 */
bool GameLogic::RemoveFromObjList(GameObject* obj){
	//Iterator for traversing object list
	std::list<GameObject*>::iterator objIter;
	for(objIter = gameObjList.begin(); objIter != gameObjList.end(); ++objIter){
		if(obj->GetGUID() == (*objIter)->GetGUID()){
			break;
		}
	}

	if(objIter != gameObjList.end()){
		gameObjList.erase(objIter);
		return true;
	} else{
		char msgbuf[256];
		sprintf(msgbuf, "Cannot find active bullet instance!\n"); 
		return false;
	}
}

/**
 * Wrap a value around a min and max
 */
float GameLogic::WrapValue(float val, float min, float max){
	if(val > max){
		return min;
	} else if(val < min){
		return max;
	} else{
		return val;
	}
}

/** 
 * Wrap an angle in radians
 */
float GameLogic::WrapDir(float angle){
	while(angle > 2 * PI){
		angle -= 2 * PI;
	}

	while(angle < 0){
		angle += 2 * PI;
	}

	return angle;
}

/**
 * Check if the game object is a debug object
 */
bool GameLogic::IsDebugObject(GameObject* obj){
	if(obj->GetName().find("debug") != std::string::npos)
		return true;
	else if(obj->GetName().find("Debug") != std::string::npos)
		return true;
	else
		return false;
}