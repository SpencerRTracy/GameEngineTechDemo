#include "ObjectCreator.h"
#include <fstream>


/**
 * Class to manage the creation of all objects and models.
 * Uses text serialization to create objects.
 */
ObjectCreator::ObjectCreator() {}

/**
 * Initialize method for this component
 */
void ObjectCreator::Initialize(){
	//Create all the models given
	CreateModels();
}

/**
 * Text serialization.
 * Create a GameObject using only text files.
 */
GameObject* ObjectCreator::CreateGameObject(std::string filename){
	GameObject* gameObj = new GameObject();
	std::ifstream inputStream (filename, std::ifstream::in);
	std::string str;

	//Check if stream is good.
	if(!inputStream.good()){
		//Most likely "File Not Found"
		char msgbuf[256];
		sprintf(msgbuf, "Error reading from '%s'.  Most likely File Not Found.  Creating Game Object with default values...", filename); 
	} else {
		if(inputStream.eof() == 0){
			//Set the object name
			std::getline(inputStream, str);
			gameObj->SetName(str);
		}

		if(inputStream.eof() == 0){
			//Set the model type
			std::getline(inputStream, str);
			gameObj->SetModelName(str);
		}

		if(inputStream.eof() == 0){
			//Set initial X, Y, & Z position
			std::getline(inputStream, str, ',');
			gameObj->SetXPos( (float) std::atof(str.c_str()) );

			std::getline(inputStream, str, ',');
			gameObj->SetYPos( (float) std::atof(str.c_str()) );

			std::getline(inputStream, str);
			gameObj->SetLayer( (float) std::atof(str.c_str()) );

		}

		if(inputStream.eof() == 0){
			//Set Scale
			std::getline(inputStream, str, ',');
			gameObj->SetScaleX( (float) std::atof(str.c_str()) );

			std::getline(inputStream, str);
			gameObj->SetScaleY( (float) std::atof(str.c_str()) );

		}
		if(inputStream.eof() == 0){
			//Set Rotations
			std::getline(inputStream, str, ',');
			gameObj->SetRotX( (float) std::atof(str.c_str()) );

			std::getline(inputStream, str);
			gameObj->SetRotY( (float) std::atof(str.c_str()) );

		}

		if(inputStream.eof() == 0){
			//Set the object's Collision Type
			std::getline(inputStream, str);
			int collisionType;
			if(str == "Rectangle"){
				collisionType = 1;
			} else if(str == "Circle"){
				collisionType = 2;
			} else {
				collisionType = 0;
			}
			gameObj->SetCollisionType(collisionType);
		}
	}

	return gameObj;	
}

/**
 * Create all the models that are going to be used.
 */
void ObjectCreator::CreateModels(){
	//All models must be listed below to be loaded for use
	CreatePyramid();
	CreateShip();
	CreateShipHit();
	CreateBG();
	CreateBullet();
	CreateMissile();
	CreateAsteroid();
	CreateFireAsteroid();
	CreateTextDisplay();
	CreateSmallTextDisplay();
	CreateDebugRect();
	CreateDebugCircle();
	CreateDebugCirclePiece();
}

void ObjectCreator::CreatePyramid(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[] =
	{
		{D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xffffffff, 0.5f, 0.0f},
        {D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff, 0.0f, 1.0f},
        {D3DXVECTOR3(1.0f,-1.0f,-1.0f), 0xffffffff, 1.0f, 1.0f},
        {D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xffffffff, 0.5f, 0.0f},
        {D3DXVECTOR3(-1.0f,-1.0f, 1.0f), 0xffffffff, 0.0f, 1.0f},
        {D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff, 1.0f, 1.0f},
        {D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xffffffff, 0.5f, 0.0f},
        {D3DXVECTOR3(1.0f,-1.0f, 1.0f), 0xffffffff, 0.0f, 1.0f},
        {D3DXVECTOR3(-1.0f,-1.0f, 1.0f), 0xffffffff, 1.0f, 1.0f},
        {D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xffffffff, 0.5f, 0.0f},
        {D3DXVECTOR3(1.0f,-1.0f,-1.0f), 0xffffffff, 0.0f, 1.0f},
        {D3DXVECTOR3(1.0f,-1.0f, 1.0f), 0xffffffff, 1.0f, 1.0f},
	};

	model->SetTriangleCoords(triangle, 4, sizeof(triangle));

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,-1.0f,-1.0f), 0xffffffff, 1.0f, 0.0f},
		{D3DXVECTOR3(-1.0f,-1.0f, 1.0f), 0xffffffff, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 1.0f), 0xffffffff, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	model->SetTextureFilename("Textures/BrownScales.jpg");
	graphics->AddModel(model, "Pyramid");
}

void ObjectCreator::CreateBG(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,1.0f,0.0f), 0xffffffff, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,1.0f,0.0f), 0xffffffff, 1.0f, 0.0f},
		{D3DXVECTOR3(-1.0f,-1.0f, 0.0f), 0xffffffff, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 0.0f), 0xffffffff, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	model->SetTextureFilename("Textures/GalaxyBG.jpg");
	graphics->AddModel(model, "Background");

}

void ObjectCreator::CreateShip(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();
	Vertex triangle[] =
	{
		{D3DXVECTOR3(-1.0f, 1.0f, 0.0f), 0xFFAFAF00, 1.0f, 1.0f}, 
		{D3DXVECTOR3(-1.0f, -1.0f, 0.0f), 0xFFAFAF00, 1.0f, 1.0f},
		{D3DXVECTOR3(1.0f,  0.0f, 0.0f), 0xFFFFFFFF, 0.0f, 0.0f}
	};

	model->SetTriangleCoords(triangle, 1, sizeof(triangle));

	Vertex quad[1];

	model->SetQuadCoords(quad, 0, 0);

	graphics->AddModel(model, "Ship");

}

void ObjectCreator::CreateShipHit(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();
	Vertex triangle[] =
	{
		{D3DXVECTOR3(-1.0f, 1.0f, 0.0f), 0xFFFF0000, 1.0f, 1.0f}, 
		{D3DXVECTOR3(-1.0f, -1.0f, 0.0f), 0xFFFF0000, 1.0f, 1.0f},
		{D3DXVECTOR3(1.0f,  0.0f, 0.0f), 0xFFFFFFFF, 0.0f, 0.0f}
	};

	model->SetTriangleCoords(triangle, 1, sizeof(triangle));

	Vertex quad[1];

	model->SetQuadCoords(quad, 0, 0);

	graphics->AddModel(model, "ShipHit");
}

void ObjectCreator::CreateBullet(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,1.0f,0.0f), 0xffffffff, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,1.0f,0.0f), 0xffffffff, 1.0f, 0.0f},
		{D3DXVECTOR3(-1.0f,-1.0f, 0.0f), 0xffffffff, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 0.0f), 0xffffffff, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	graphics->AddModel(model, "Bullet");
}

void ObjectCreator::CreateMissile(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,0.25f,0.0f), 0xffffffff, 0.0f, 0.25f},
		{D3DXVECTOR3(1.0f,0.25f,0.0f), 0xffffffff, 1.0f, 0.25f},
		{D3DXVECTOR3(-1.0f,-0.25f, 0.0f), 0xffffffff, 0.0f, 0.75f},
		{D3DXVECTOR3(1.0f,-0.25f, 0.0f), 0xffffffff, 1.0f, 0.75f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	model->SetTextureFilename("Textures/Missile.png");
	graphics->AddModel(model, "Missile");
}

void ObjectCreator::CreateAsteroid(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[] = 
	{
		//Top Half
		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.375f,  0.0f, 0.0f), 0xFF8b8989, 1.0f, 0.5f},
		{D3DXVECTOR3(0.25f,  0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.33333f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.25f,  0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.33333f},
		{D3DXVECTOR3(0.0f,  0.375f, 0.0f), 0xFFcdb79e, 0.5f, 0.0f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.0f,  0.375f, 0.0f), 0xFFcdb79e, 0.5f, 0.0f},
		{D3DXVECTOR3(-0.25f,  0.25f, 0.0f), 0xFF708090, 0.33333f, 0.33333f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.25f,  0.25f, 0.0f), 0xFF708090, 0.33333f, 0.33333f},
		{D3DXVECTOR3(-0.375f,  0.0f, 0.0f), 0xFF8b8989, 0.0f, 0.5f},

		//Bottom Half
		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.375f,  0.0f, 0.0f), 0xFF8b8989, 0.0f, 0.5f},
		{D3DXVECTOR3(-0.25f, - 0.25f, 0.0f), 0xFFcdb79e, 0.33333f, 0.66666f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.25f, - 0.25f, 0.0f), 0xFFcdb79e, 0.33333f, 0.66666f},
		{D3DXVECTOR3(0.0f,  -0.375f, 0.0f), 0xFFcdb79e, 0.5f, 1.0f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.0f,  -0.375f, 0.0f), 0xFFcdb79e, 0.5f, 1.0f},
		{D3DXVECTOR3(0.25f,  -0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.66666f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.25f,  -0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.66666f},
		{D3DXVECTOR3(0.375f,  0.0f, 0.0f), 0xFF8b8989, 1.0f, 0.5f},

	};

	//Normalize
	for(int i = 0; i < 24; i++){
		triangle[i].position.x *= 1.0f/0.375f;
		triangle[i].position.y *= 1.0f/0.375f;
	}

	model->SetTriangleCoords(triangle, 8, sizeof(triangle));

	Vertex quad[1];

	model->SetQuadCoords(quad, 0, 0);

	graphics->AddModel(model, "Asteroid");
}

void ObjectCreator::CreateFireAsteroid(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[] = 
	{
		//Top Half
		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFFF5050, 0.5f, 0.5f},
		{D3DXVECTOR3(0.375f,  0.0f, 0.0f), 0xFF8b8989, 1.0f, 0.5f},
		{D3DXVECTOR3(0.25f,  0.25f, 0.0f), 0xFFFF5050, 0.66666f, 0.33333f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.25f,  0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.33333f},
		{D3DXVECTOR3(0.0f,  0.375f, 0.0f), 0xFFFF5050, 0.5f, 0.0f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFFF5050, 0.5f, 0.5f},
		{D3DXVECTOR3(0.0f,  0.375f, 0.0f), 0xFFFF5050, 0.5f, 0.0f},
		{D3DXVECTOR3(-0.25f,  0.25f, 0.0f), 0xFFFF5050, 0.33333f, 0.33333f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.25f,  0.25f, 0.0f), 0xFFFF5050, 0.33333f, 0.33333f},
		{D3DXVECTOR3(-0.375f,  0.0f, 0.0f), 0xFFFF5050, 0.0f, 0.5f},

		//Bottom Half
		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.375f,  0.0f, 0.0f), 0xFF8b8989, 0.0f, 0.5f},
		{D3DXVECTOR3(-0.25f, - 0.25f, 0.0f), 0xFFcdb79e, 0.33333f, 0.66666f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(-0.25f, - 0.25f, 0.0f), 0xFFcdb79e, 0.33333f, 0.66666f},
		{D3DXVECTOR3(0.0f,  -0.375f, 0.0f), 0xFFcdb79e, 0.5f, 1.0f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.0f,  -0.375f, 0.0f), 0xFFcdb79e, 0.5f, 1.0f},
		{D3DXVECTOR3(0.25f,  -0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.66666f},

		{D3DXVECTOR3(0.0f,  0.0f, 0.0f), 0xFFcdb79e, 0.5f, 0.5f},
		{D3DXVECTOR3(0.25f,  -0.25f, 0.0f), 0xFFcdb79e, 0.66666f, 0.66666f},
		{D3DXVECTOR3(0.375f,  0.0f, 0.0f), 0xFF8b8989, 0.0f, 0.5f},

	};

	//Normalize
	for(int i = 0; i < 24; i++){
		triangle[i].position.x *= 1.0f/0.375f;
		triangle[i].position.y *= 1.0f/0.375f;
	}

	model->SetTriangleCoords(triangle, 8, sizeof(triangle));

	Vertex quad[1];

	model->SetQuadCoords(quad, 0, 0);

	model->SetTextureFilename("Textures/lava.png");
	graphics->AddModel(model, "FireAsteroid");
}

void ObjectCreator::CreateTextDisplay(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	//RECT to hold font position
	RECT fontPos;
	fontPos.top = 0;
	fontPos.bottom = graphics->GetViewportHeight();
	fontPos.right = graphics->GetViewportWidth();
	fontPos.left = 0;

	//Set model type to 0 to signify its not a primitive
	model->SetType(0);
	model->SetRect(fontPos);
	model->SetWidth(graphics->GetViewportWidth());
	model->SetHeight(graphics->GetViewportHeight());
	graphics->AddModel(model, "TextDisplay");

}

void ObjectCreator::CreateSmallTextDisplay(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	//RECT to hold font position
	RECT fontPos;
	fontPos.top = 0;
	fontPos.bottom = graphics->GetViewportHeight();
	fontPos.right = graphics->GetViewportWidth();
	fontPos.left = 0;

	//Set model type to 0 to signify its not a primitive
	model->SetType(1);
	model->SetRect(fontPos);
	model->SetWidth(graphics->GetViewportWidth());
	model->SetHeight(graphics->GetViewportHeight());
	graphics->AddModel(model, "SmallTextDisplay");
}

void ObjectCreator::CreateDebugRect(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,1.0f,0.0f), 0xffff0000, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,1.0f,0.0f), 0xffff0000, 1.0f, 0.0f},
		{D3DXVECTOR3(-1.0f,-1.0f, 0.0f), 0xffff0000, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 0.0f), 0xffff0000, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	model->SetTextureFilename("Textures/SquareOutline.png");
	graphics->AddModel(model, "DebugRect");
}

void ObjectCreator::CreateDebugCircle(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(-1.0f,1.0f,0.0f), 0x00FF0000, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,1.0f,0.0f), 0x00FF0000, 1.0f, 0.0f},
		{D3DXVECTOR3(-1.0f,-1.0f, 0.0f), 0x00FF0000, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 0.0f), 0x00FF0000, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	model->SetTextureFilename("Textures/CircleOutline.png");
	graphics->AddModel(model, "DebugCircle");
}

void ObjectCreator::CreateDebugCirclePiece(){
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(ENGINE->GetComponent("GraphicsSystem"));
	ObjectModel* model = new ObjectModel();

	Vertex triangle[1];

	model->SetTriangleCoords(triangle, 0, 0);

	Vertex quad[] =
	{
		{D3DXVECTOR3(0.0f,1.0f,0.0f), 0xff00aa00, 0.0f, 0.0f},
		{D3DXVECTOR3(1.0f,1.0f,0.0f), 0xff00aa00, 1.0f, 0.0f},
		{D3DXVECTOR3(0.0f,-1.0f, 0.0f), 0xff00aa00, 0.0f, 1.0f},
		{D3DXVECTOR3(1.0f,-1.0f, 0.0f), 0xff00aa00, 1.0f, 1.0f}
	};

	model->SetQuadCoords(quad, 1, sizeof(quad));

	graphics->AddModel(model, "DebugCirclePiece");
}