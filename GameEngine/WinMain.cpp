
#include "WindowsSystem.h"
#include "GraphicsSystem.h"
#include "ObjectCreator.h"
#include "GameLogic.h"
#include "MainEngine.h"
#include "EventManager.h"
#include "Physics.h"
#include "InputManager.h"

//Height of window
#define WINDOW_WIDTH 800
//Width of window
#define WINDOW_HEIGHT 600

/**
 * Main function
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	//Create the Engine
	MainEngine* engine = new MainEngine();

	//////////////////////////////////////////////////////////////////////////////////////////////
	//ADD ALL COMPONENTS TO ENGINE BELOW
	///////////////////////////////////////////////////////////////////////////////////////////////
	WindowsSystem* windows = new WindowsSystem("Game Title", WINDOW_WIDTH, WINDOW_HEIGHT);
	engine->AddComponent(windows, "WindowsSystem");

	ObjectCreator* objectCreator = new ObjectCreator();
	engine->AddComponent(objectCreator, "ObjectCreator");

	GraphicsSystem* graphics = new GraphicsSystem(windows->GetHWND(), windows->getWidth(), windows->getHeight());
	engine->AddComponent(graphics, "GraphicsSystem");

	GameLogic* gameLogic = new GameLogic(windows->getWidth(), windows->getHeight());
	engine->AddComponent(gameLogic, "GameLogic");

	EventManager* eventManager = new EventManager();
	engine->AddComponent(eventManager, "EventManager");

	Physics* physics = new Physics();
	engine->AddComponent(physics, "Physics");

	InputManager* inputManager = new InputManager();
	engine->AddComponent(inputManager, "InputManager");



	//Initialize all of the components now that they've been added
	engine->InitializeComponents();

	//Loop until Windows passes a quit response
	while(true){
		if(windows->UpdateWindowsLoop() == TRUE){
			return 0;
		}

		//Update all engine components
		engine->UpdateComponents();
	}
}