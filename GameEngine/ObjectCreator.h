#pragma once

#include "MainEngine.h"
#include "ObjectModel.h"
#include "EventManager.h"

#include <d3dx9.h>
#include <d3d9.h>


/**
 * Class to manage the creation of all objects and models.
 * Uses text serialization to create objects.
 */
class ObjectCreator : public Component{
public:
	/**
	 * Class to manage the creation of all objects and models.
	 * Uses text serialization to create objects.
	 */
	ObjectCreator();
	~ObjectCreator(){}

	/**
	 * Create all the models that are going to be used.
	 */
	void CreateModels();

	/**
	 * Text serialization.
	 * Create a GameObject using only text files.
	 */
	GameObject* CreateGameObject(std::string filename);
	
	/**
	 * Initialize method for this component
	 */
	void Initialize();

private:

	//Model Creation Here
	void CreateMonstrosity();
	void CreatePyramid();
	void CreateShip();
	void CreateShipHit();
	void CreateBG();
	void CreateBullet();
	void CreateMissile();
	void CreateAsteroid();
	void CreateFireAsteroid();
	void CreateTextDisplay();
	void CreateSmallTextDisplay();
	void CreateDebugRect();
	void CreateDebugCircle();
	void CreateDebugCirclePiece();

};