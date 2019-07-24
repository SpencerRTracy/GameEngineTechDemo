#include "WindowsSystem.h"

#define WIDTH 1000
#define HEIGHT 800

WindowsSystem* windowsSystem;

/**
 * Process windows messages
 */
LRESULT CALLBACK WindowsMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
        case WM_DESTROY:
            //Close the application
            PostQuitMessage(0);
            return 0;
            break;
		case WM_KEYDOWN:
			//Send Key message to InputManager
			windowsSystem->eventManager->SendMessage("InputManager", windowsSystem->eventManager->KEY_DOWN, (void*) wParam);
			break;
		case WM_KEYUP:
			//Send Key message to InputManager
			windowsSystem->eventManager->SendMessage("InputManager", windowsSystem->eventManager->KEY_UP, (void*) wParam);
			break;
    }

    //Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}

/**
 * Create the WindowsSystem component
 */
WindowsSystem::WindowsSystem(const char* windowTitle, int clientWidth, int clientHeight){
	//Get the width and height
	this->clientWidth = clientWidth;
	this->clientHeight = clientHeight;

	//Create class name
	const char windowsClassName[] = "WindowClass";

	//Struct holds information for the window class
	WNDCLASSEX wc;

	//Clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//Fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowsMessageProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = windowsClassName;

	//Register the window class
	RegisterClassEx(&wc);

	//Adjust Client Size Area
	RECT clientArea = {0, 0 , WIDTH, HEIGHT};
	AdjustWindowRect(&clientArea, WS_OVERLAPPEDWINDOW, FALSE);
	
	//Create the window and use the result as the handle
	hWnd = CreateWindowEx(	NULL,
							windowsClassName,
							windowTitle,
							WS_OVERLAPPEDWINDOW,
							300,
							300,
							clientArea.right - clientArea.left,
							clientArea.bottom - clientArea.top,
							NULL,
							NULL,
							hInstance,
							NULL);

	//Display the window on the screen
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

/**
 * Initialize method for this component
 */
void WindowsSystem::Initialize(){
	windowsSystem = this;
	eventManager = dynamic_cast<EventManager*>(ENGINE->GetComponent("EventManager"));
}

/**
 * Update loop for Windows
 */
int WindowsSystem::UpdateWindowsLoop(){
	//Enter the main loop
	bool exitTime = false;
	
	//Struct to hold Windows event messages
	MSG msg;

	//Check the next message
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		//Translate keystroke messages into the right format
		TranslateMessage(&msg);

		//Send the message to the WindowProc function
		DispatchMessage(&msg);
		
		if(msg.message == WM_QUIT){
			exitTime = true;
		}
	}

	//Return the Quit exit bool
	return exitTime;
}

WindowsSystem::~WindowsSystem(){

}

///////////////////////////////////////////
//GETTERS AND SETTERS
//////////////////////////////////////////
HWND WindowsSystem::GetHWND(){
	return hWnd;
}

int WindowsSystem::getWidth(){
	return WIDTH;
}

int WindowsSystem::getHeight(){
	return HEIGHT;
}