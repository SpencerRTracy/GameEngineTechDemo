#include "Physics.h"

/**
 * Component to handle all collision checking and
 * Movement responses to collision.
 */
Physics::Physics() : newCollisionCheck(false) {}

Physics::~Physics(){}


/**
 * Initialize method for this component
 */
void Physics::Initialize(){
	eventManager = dynamic_cast<EventManager*>(ENGINE->GetComponent("EventManager"));
}
/**
 * Update loop
 */
void Physics::Update(){

}

/**
 * Update loop that needs to happen immediately.
 * Used for collision checks.
 */
void Physics::UpdateNow(){
	HandleMessages();

	//Perform collision checks
	if(newCollisionCheck){
		float scaler = 14.0f;
		std::list<GameObject*>::iterator iter1 = gameObjList.begin();
		while(iter1 != gameObjList.end()){
			std::list<GameObject*>::iterator iter2 = gameObjList.begin();
			while(iter2 != gameObjList.end()){
				//Check to see if both objects are collidable
				if((*iter1)->GetGUID() != (*iter2)->GetGUID()  
					&& ( (*iter1)->GetCollisionType() != COLTYPE_NONE && (*iter2)->GetCollisionType() != COLTYPE_NONE) 
					&& HasNotBeenChecked(*iter1)){
					//Check for collisions
					Vector2D center1 = {(*iter1)->GetXPos(), (*iter1)->GetYPos()};
					Vector2D center2 = {(*iter2)->GetXPos(), (*iter2)->GetYPos()};

					if((*iter1)->GetCollisionType() == COLTYPE_RECTANGLE){
						//Object 1 is a rectangle
						float width1 = (*iter1)->GetScaleX()*2 * scaler * 0.8f; 
						float height1 = (*iter1)->GetScaleX()*2 * scaler * 0.8f; 

						if((*iter2)->GetCollisionType() == COLTYPE_RECTANGLE){
							//Rect to Rect collision check
							float width2 = (*iter2)->GetScaleX()*2 * scaler * 0.8f;
							float height2 = (*iter2)->GetScaleX()*2 * scaler * 0.8f; 

							if(StaticRectToStaticRect(&center1, width1, height1, &center2, width2, height2)){
								SendCollisionMessage(*iter1, *iter2);
							}

						} else if((*iter2)->GetCollisionType() == COLTYPE_CIRCLE){
							//Rect to Circle collision check
							float radius2 = ((*iter2)->GetScaleX() + (*iter2)->GetScaleY()) / 2.0f;
							radius2 *= scaler;

							if(StaticCircleToStaticRectangle(&center2, radius2, &center1, width1, height1)){
								SendCollisionMessage(*iter1, *iter2);
							}
						}
					} else if((*iter1)->GetCollisionType() == COLTYPE_CIRCLE && (*iter1)->GetName() != "FireAsteroid"){
						//Object 1 is a circle
						float radius1 = ((*iter1)->GetScaleX() + (*iter1)->GetScaleY()) / 2.0f;
						radius1 *= scaler;

						if((*iter2)->GetCollisionType() == COLTYPE_RECTANGLE){
							//Circle to Rect collision check
							//Rect to Rect collision check
							float width2 = (*iter2)->GetScaleX()*2 * scaler * 0.8f; 
							float height2 = (*iter2)->GetScaleX()*2 * scaler * 0.8f; 

							if(StaticCircleToStaticRectangle(&center1, radius1, &center2, width2, height2)){
								SendCollisionMessage(*iter1, *iter2);
							}

						} else if((*iter2)->GetCollisionType() == COLTYPE_CIRCLE  && (*iter2)->GetName() != "FireAsteroid"){
							//Circle to Circle collision check
							float radius2 = ((*iter2)->GetScaleX() + (*iter2)->GetScaleY()) / 2.0f;
							radius2 *= scaler;
							GameObject* obj2 = *iter2;
							GameObject* obj1 = *iter1;
							
							if(StaticCircleToStaticCircle(&center1, radius1, &center2, radius2)){
								Vector2D obj1Final = {obj1->GetXPos(), obj1->GetYPos()};
								Vector2D obj2Final = {obj2->GetXPos(), obj2->GetYPos()};
								{
									Vector2D unitVector;
									Vector2D scaledVector;
									Vector2DFromAngleRad(&unitVector, obj1->GetCurrentDir());
									Vector2DScale(&scaledVector, &unitVector, -obj1->GetSpeed());

									Vector2D newPosition;
									Vector2D oldPosition = {obj1->GetXPos(), obj1->GetYPos()};
									Vector2DAdd(&newPosition, &oldPosition, &scaledVector);
									obj1->SetXPos(newPosition.x);
									obj1->SetYPos(newPosition.y);
								}
								{
									Vector2D unitVector;
									Vector2D scaledVector;
									Vector2DFromAngleRad(&unitVector, obj2->GetCurrentDir());
									Vector2DScale(&scaledVector, &unitVector, -obj2->GetSpeed());

									Vector2D newPosition;
									Vector2D oldPosition = {obj2->GetXPos(), obj2->GetYPos()};
									Vector2DAdd(&newPosition, &oldPosition, &scaledVector);
									obj2->SetXPos(newPosition.x);
									obj2->SetYPos(newPosition.y);
								}
			
								Vector2D obj1Initial = {obj1->GetXPos(), obj1->GetYPos()};
								Vector2D obj2Initial = {obj2->GetXPos(), obj2->GetYPos()};

								{
									//Create vectors
									Vector2D N;
									Vector2D V;
									Vector2D reflect;

										
									//Find normal
									Vector2DSub(&N, &center1, &center2);
									float x = N.x;
									float y = N.y;
									N.x = -y;
									N.y = x;
									Vector2DNormalize(&N, &N);
										
									
									//Get velocity vector
									Vector2DSub(&V, &obj2Final, &obj2Initial);
									
									//Get reflected velocity vector
									Vector2DScaleAdd(&reflect, &N, &V, -2.0f * Vector2DDotProduct(&V, &N));

									float angle = atan(reflect.y/reflect.x);
									if(reflect.x > 0){
										//angle = angle;
									} else if(reflect.x < 0){
										angle -= PI;
									} else if(reflect.x == 0){
										if(reflect.y > 0){
											angle = PI;
										} else if(reflect.y < 0){
											angle = -PI;
										}
									}
									

									obj2->SetXPos(obj2->GetXPos() + reflect.x);
									obj2->SetYPos(obj2->GetYPos() + reflect.y);
									obj2->SetCurrentDir(angle);
								}

								{
									//Create vectors
									Vector2D N;
									Vector2D V;
									Vector2D reflect;

										
									//Find normal
									Vector2DSub(&N, &center1, &center2);
									float x = N.x;
									float y = N.y;
									N.x = -y;
									N.y = x;
									Vector2DNormalize(&N, &N);
									
									//Get velocity vector
									Vector2DSub(&V, &obj1Final, &obj1Initial);

									//Get reflected velocity vector
									Vector2DScaleAdd(&reflect, &N, &V, -2.0f * Vector2DDotProduct(&V, &N));

									float angle = atan(reflect.y/reflect.x);
									if(reflect.x > 0){
										//angle = angle;
									} else if(reflect.x < 0){
										angle -= PI;
									} else if(reflect.x == 0){
										if(reflect.y > 0){
											angle = PI;
										} else if(reflect.y < 0){
											angle = -PI;
										}
									}

									
									obj1->SetXPos(obj1->GetXPos() + reflect.x);
									obj1->SetYPos(obj1->GetYPos() + reflect.y);
									obj1->SetCurrentDir(angle);
								}
							}
						}
					}
				}
				++iter2;
			}	
			++iter1;
		}
	}

	newCollisionCheck = false;
	
	//Clear the list of objects already checked in preparation of next update
	alreadyCheckedVec.clear();

	//Clear the object list in preparation of next update
	gameObjList.clear();
}

void Physics::ReceiveMessage(Message* msg){
	//Add message to mailbox (Message List)
	messageList.push_back(msg);
	if(msg->Read() == EventManager::PHYSICS_CHECK){
		UpdateNow();
	}

}

/**
 * Handle standard messages
 */
void Physics::HandleMessages(){
	std::list<Message*>::iterator iter = messageList.begin();
	//Empty "Mail Box"
	while(messageList.size() != 0){
		//Handle Messages
		Message* msg = messageList.front();
		messageList.pop_front();
		switch (msg->Read()){
			case EventManager::PHYSICS_CHECK:
				newCollisionCheck = true;
				gameObjList =  *((std::list<GameObject*>*) msg->GetData());
		}
		delete msg;
	}
}

/**
 * Check if the given GameObject has already been checked by physics
 */
bool Physics::HasNotBeenChecked(GameObject* obj){
	int testGUID = obj->GetGUID();
	for(unsigned int i = 0; i < alreadyCheckedVec.size(); i++){
		if(alreadyCheckedVec[i] == testGUID)
			return false;
	}
	return true;
}

/**
 * Send out a collision event message.
 * Send the two GameObjects as message data
 */
void Physics::SendCollisionMessage(GameObject* obj1, GameObject* obj2){
	std::pair<GameObject*, GameObject*>* colPair;

	//Add to already checked vector
	alreadyCheckedVec.push_back(obj1->GetGUID());
	alreadyCheckedVec.push_back(obj2->GetGUID());
								
	//Alphabetize for consistency
	if(obj1->GetName() < obj2->GetName()){
		colPair = new std::pair<GameObject*, GameObject*>(obj1, obj2);
	} else {
		colPair = new std::pair<GameObject*, GameObject*>(obj2, obj1);
	}

	//Send the message
	eventManager->SendMessageNow("GameLogic", EventManager::COLLISION, (void*) colPair);
}