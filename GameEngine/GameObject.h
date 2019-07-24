#pragma once

extern "C"
{
#include "Matrix2D.h"
}

#include <string>

//Forward declaration
class ObjectModel;


/**
 * Class to hold all game objects
 */
class GameObject{
public:

	/**
	 * Class to hold all game objects
	 */
	GameObject();
	
	/**
	 * Copy constructor for creating object instances after serialization.
	 * Every instance will have a unique GUID.
	 */
	GameObject(const GameObject& rhs);
	~GameObject();

	/**
	 * Static instance counter for automatically generating GUIDs
	 */
	static int instanceCounter;

	////////////////////////////////////
	//Getters and Setters
	///////////////////////////////////

	void SetName(std::string newName);
	std::string GetName();

	void SetModelName(std::string newName);
	std::string GetModelName();

	void SetXPos(float newX);
	float GetXPos();

	void SetYPos(float newY);
	float GetYPos();

	void SetLayer(float newLayer);
	float GetLayer();

	void SetXVelocity(float newXVel);
	float GetXVelocity();

	void SetYVelocity(float newYVel);
	float GetYVelocity();

	void SetSpeed(float newSpeed);
	float GetSpeed();

	void SetScaleX(float newX);
	float GetScaleX();

	void SetScaleY(float newY);
	float GetScaleY();

	void SetRotX(float newX);
	float GetRotX();

	void SetRotY(float newY);
	float GetRotY();

	void SetRotZ(float newZ);
	float GetRotZ();

	void SetCurrentDir(float newZ);
	float GetCurrentDir();
	
	void SetCollisionType(int newC);
	int GetCollisionType();

	Matrix2D* GetTransMat();

	int GetGUID();

	bool HasMultiplied();
	void SetMultiplied(bool newVal);

	void SetText(std::string newText);
	std::string GetText();
	
	bool multiplied;

private:
	std::string name;
	int GUID;

	//Position Values
	float xPos;
	float yPos;
	float zPos;

	//Velocity Values
	float xVel;
	float yVel;
	float speed;

	//Scale values
	float scaleX;
	float scaleY;
	
	//Rotation values
	float rotX;
	float rotY;
	float rotZ;
	
	//Angle representation of object
	float currentDir;	

	//Type of collision for the object
	int collisionType;

	//Text to Display
	std::string text;

	//String to hold the model used for this game object
	std::string modelName;
};