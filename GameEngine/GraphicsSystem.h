#include <d3dx9.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include <vector>

#pragma once
#include "GameObject.h"
#include "Component.h"
#include <unordered_map>

//Forward declaration
class ObjectModel;

/**
 * Component responsible for graphics and all the draw calls
 */
class GraphicsSystem : public Component{
public:
	/**
	 * Sets up and initializes Direct3D
	 */
	GraphicsSystem(HWND hWnd, int clientWidth, int clientHeight);
	
	/**
	 * Release DirectX
	 */
	~GraphicsSystem();

	/**
	 * Initialize method for this component
	 */
	void Initialize();

	/**
	 * Update loop
	 */
	void Update();

	/**
	 * Add a model to the hash map and vector for easy access by Graphics
	 */
	void AddModel(ObjectModel* model, std::string modelName);

	/** 
	 * Updates the active list of game objects to draw to the screen.
	 * Uses already supplied models.
	 */
	void UpdateGameObjects(std::list<GameObject*>* newObjList);

	///////////////////////////////////////////
	//GETTERS AND SETTERS
	//////////////////////////////////////////
	int GetViewportWidth();
	int GetViewportHeight();

private:
	typedef std::unordered_map<std::string, ObjectModel*> ObjectModelMap;

	//Pointer to Windows HWND
	HWND hWnd;

	//Pointer to Direct3D Device
	IDirect3DDevice9* device;

	//Pointer to Direct3D Device Context
	IDirect3D9* deviceCon;

	//View Matrix
	D3DXMATRIXA16 viewMatrix;

	//Game Object list to hold all objects that need to be drawn
	std::list<GameObject*> gameObjList;
	//Vector holding all loaded models
	std::vector<ObjectModel*> modelVec;
	//Hash Map holding all loaded models
	ObjectModelMap modelMap;

	//Viewport values
	int viewportWidth;
	int viewportHeight;

	/**
	 * Load all assets into Graphics memory
	 */
	void LoadAssets();

	/**
	 * Set the model's vertex buffers
	 */
	void SetVertexBuffers(ObjectModel* currentModel);
	
	/** 
	 * Create a texture from a filename and set it to the model
	 */
	void SetTextures(ObjectModel* currentModel);
	
	/**
	 * Set up the world view and camera
	 */
	void SetupWorldProj();

};