#include "GameObject.h"

/**
 * Static instance counter.
 * Used to automatically create GUIDs
 */
int GameObject::instanceCounter = -1;

/**
 * Class to hold all game objects
 */
GameObject::GameObject() :	GUID(++instanceCounter),
									xPos(0),
									yPos(0),
									zPos(50.0f),
									xVel(0.0f),
									yVel(0.0f),
									scaleX(1),
									scaleY(1),
									rotX(0),
									rotY(0),
									rotZ(0),
									currentDir(0),
									collisionType(0),
									name("NULL"),
									modelName("NULL"),
									text("NULL"),
									multiplied(false)
{

}

GameObject::~GameObject(){
}

/**
 * Copy constructor for creating object instances after serialization.
 * Every instance will have a unique GUID.
 */
GameObject::GameObject(const GameObject& rhs){

	//IDs
	name = rhs.name;
	GUID = ++instanceCounter;

	//Position Values
	xPos = rhs.xPos;
	yPos = rhs.yPos;
	zPos = rhs.zPos;

	//Velocity Values
	xVel = rhs.xVel;
	yVel = rhs.yVel;
	speed = rhs.speed;

	//Scale values
	scaleX = rhs.scaleX;
	scaleY = rhs.scaleY;
	
	//Rotation values
	rotX = rhs.rotX;
	rotY = rhs.rotY;
	rotZ = rhs.rotZ;
	
	//Angle representation of object
	currentDir = rhs.currentDir;

	//Model to use for graphics
	modelName = rhs.modelName;

	//Text to display
	text = rhs.text;

	//Collision Type for Physics
	collisionType = rhs.collisionType;

	multiplied = rhs.multiplied;
}

void GameObject::SetName(std::string newName){
	name = newName;
}

std::string GameObject::GetName(){
	return name;
}

void GameObject::SetModelName(std::string newName){
	modelName = newName;
}

std::string GameObject::GetModelName(){
	return modelName;
}

void GameObject::SetXPos(float newX){
	xPos = newX;
}

float GameObject::GetXPos(){
	return xPos;
}

void GameObject::SetYPos(float newY){
	yPos = newY;
}

float GameObject::GetYPos(){
	return yPos;
}

void GameObject::SetLayer(float newLayer){
	zPos = newLayer;
}

float GameObject::GetLayer(){
	return zPos;
}

void GameObject::SetXVelocity(float newXVel){
	xVel = newXVel;
}

float GameObject::GetXVelocity(){
	return xVel;
}

void GameObject::SetYVelocity(float newYVel){
	yVel = newYVel;
}

float GameObject::GetYVelocity(){
	return yVel;
}

void GameObject::SetSpeed(float newSpeed){
	speed = newSpeed;
}

float GameObject::GetSpeed(){
	return speed;
}

void GameObject::SetScaleX(float newX){
	scaleX = newX;
}

float GameObject::GetScaleX(){
	return scaleX;
}

void GameObject::SetScaleY(float newY){
	scaleY = newY;
}

float GameObject::GetScaleY(){
	return scaleY;
}

void GameObject::SetRotX(float newX){
	rotX = newX;
}

float GameObject::GetRotX(){
	return rotX;
}

void GameObject::SetRotY(float newY){
	rotY = newY;
}

float GameObject::GetRotY(){
	return rotY;
}

void GameObject::SetRotZ(float newZ){
	rotZ = newZ;
}

float GameObject::GetRotZ(){
	return rotZ;
}

void GameObject::SetCurrentDir(float newDir){
	currentDir = newDir;
}

float GameObject::GetCurrentDir(){
	return currentDir;
}

void GameObject::SetCollisionType(int newC){
	collisionType = newC;
}

int GameObject::GetCollisionType(){
	return collisionType;
}

Matrix2D* GameObject::GetTransMat(){
	Matrix2D* pTransMat = new Matrix2D;
	Matrix2DTranslate(pTransMat, xPos, yPos);
	return pTransMat;
}

int GameObject::GetGUID(){
	return GUID;
}

void GameObject::SetMultiplied(bool newVal){
	multiplied = newVal;
}

bool GameObject::HasMultiplied(){
	return multiplied;
}

void GameObject::SetText(std::string newText){
	text = newText;
}

std::string GameObject::GetText(){
	return text;
}