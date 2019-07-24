
#pragma once //Makes sure this header is only included once

#include <windows.h>
#include <windowsx.h>

#include "Component.h"
#include "EventManager.h"

/**
 * Create the WindowsSystem component
 */
class WindowsSystem: public Component{
public:
	/**
	 * Create the WindowsSystem component
	 */
	WindowsSystem(const char* windowTitle, int clientWidth, int clientHeight);
	~WindowsSystem();

	/**
	 * Initialize method for this component
	 */
	void Initialize();
	
	/**
	 * Update loop for Windows
	 */
	int UpdateWindowsLoop();

	//Event Message Handler
	EventManager* eventManager;

	///////////////////////////////////////////
	//GETTERS AND SETTERS
	//////////////////////////////////////////
	HWND GetHWND();
	int getWidth();
	int getHeight();

private:
	//Window Handle
	HWND hWnd;
	//Instance for Windows
	HINSTANCE hInstance;

	//Client area
	int clientWidth;
	int clientHeight;
};