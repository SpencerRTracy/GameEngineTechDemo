#include "GraphicsSystem.h"
#include "ObjectModel.h"

#define FVFCODE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/**
 * Sets up and initializes Direct3D
 */
GraphicsSystem::GraphicsSystem(HWND hWnd, int width, int height){
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////INITIALIZE DIRECT3D///////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	this->hWnd = hWnd;
	this->viewportWidth = width;
	this->viewportHeight = height;

    //Create device context
	deviceCon = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dParams;
	ZeroMemory(&d3dParams, sizeof(d3dParams));
	d3dParams.Windowed = TRUE;
	d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dParams.hDeviceWindow = hWnd;
	d3dParams.EnableAutoDepthStencil = true;
	d3dParams.AutoDepthStencilFormat = D3DFMT_D16; 
	d3dParams.BackBufferWidth = width;
	d3dParams.BackBufferHeight = height;
	d3dParams.BackBufferFormat = D3DFMT_R5G6B5;
	d3dParams.MultiSampleType = D3DMULTISAMPLE_NONE;

	//Create a device from the context
	deviceCon->CreateDevice(D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL,
							hWnd,
 							D3DCREATE_SOFTWARE_VERTEXPROCESSING,
 							&d3dParams,
							&device);

	//Set all the render state options
	device->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
	device->SetRenderState(D3DRS_LIGHTING,false);
	device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	device->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000008f);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

	device->SetFVF(FVFCODE);

	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////SET SAMPPLER STATE * PERSPECTIVE////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	D3DXMATRIX projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DX_PI / 4.0f, width/height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION,&projMat);

	for(unsigned i = 0;i < 8;++i)
	 {
		device->SetSamplerState(i,
								D3DSAMP_MINFILTER,
								D3DTEXF_LINEAR);
		device->SetSamplerState(i,
								D3DSAMP_MAGFILTER,
								D3DTEXF_LINEAR);
		device->SetSamplerState(i,
								D3DSAMP_MIPFILTER,
								D3DTEXF_ANISOTROPIC);
		device->SetSamplerState(i,
								D3DSAMP_MAXANISOTROPY,
								0);
	 }

	SetupWorldProj();
}

/**
 * Initialize method for this component
 */
void GraphicsSystem::Initialize(){
	LoadAssets();
}

/**
 * Load all assets into Graphics memory
 */
void GraphicsSystem::LoadAssets(){
	//Load all the model's assets (textures, etc.) and create vertex buffers
	for(unsigned int i=0; i < modelVec.size(); i++){
		SetVertexBuffers(modelVec[i]);
		SetTextures(modelVec[i]);
	}
}

/**
 * Add a model to the hash map and vector for easy access by Graphics
 */
void GraphicsSystem::AddModel(ObjectModel* model, std::string modelName){
	modelVec.push_back(model);
	modelMap.insert(std::make_pair(modelName, model));
}

/** 
 * Updates the active list of game objects to draw to the screen.
 * Uses already supplied models.
 */
void GraphicsSystem::UpdateGameObjects(std::list<GameObject*>* newObjList){
	gameObjList = *(newObjList);
}

/**
 * Set the model's vertex buffers
 */
void GraphicsSystem::SetVertexBuffers(ObjectModel* currentModel){
	//Check the type to set
	if(currentModel->GetType() == 0){
		//Type is a font

		//Font
		LPD3DXFONT font;

		//Font options
		D3DXFONT_DESC fontDesc = {50, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, "Arial"};

		//Create the font
		D3DXCreateFontIndirect(device, &fontDesc, &font);

		//Send the font to the model
		currentModel->SetFont(font);

	} else if (currentModel->GetType() == 1){
		//Type is a font

		//Font
		LPD3DXFONT font;

		//Font options
		D3DXFONT_DESC fontDesc = {25, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, "Arial"};

		//Create the font
		D3DXCreateFontIndirect(device, &fontDesc, &font);

		//Send the font to the model
		currentModel->SetFont(font);
		
	} else{
		//Create the Tri and Quad vertex buffers
		device->CreateVertexBuffer(currentModel->GetTriByteSize(), D3DUSAGE_WRITEONLY, FVFCODE, D3DPOOL_MANAGED, currentModel->GetTriVB(), NULL);       // create the buffer
		device->CreateVertexBuffer(currentModel->GetQuadByteSize(), D3DUSAGE_WRITEONLY, FVFCODE, D3DPOOL_MANAGED, currentModel->GetQuadVB(), NULL);       // create the buffer

		//Lock the vertex buffer to set the triangle vertices
		VOID* pVoid;
		if(currentModel->GetTriangleCount() > 0){
			(*currentModel->GetTriVB())->Lock(0, 0, (void**)&pVoid, 0);

			//Copy in the vertices
			memcpy(pVoid, currentModel->GetTriangleCoords(), currentModel->GetTriByteSize());

			//Unlock the vertex buffer
			(*currentModel->GetTriVB())->Unlock();
		}

		//Lock the vertex buffer to set the quad vertices
		if(currentModel->GetQuadCount() > 0){
			(*currentModel->GetQuadVB())->Lock(0, 0, (void**)&pVoid, 0);
			
			//Copy in the vertices
			memcpy(pVoid, currentModel->GetQuadCoords(), currentModel->GetQuadByteSize());

			//Unlock the vertex buffer
			(*currentModel->GetQuadVB())->Unlock();
		}
	}

}

/** 
 * Create a texture from a filename and set it to the model
 */
void GraphicsSystem::SetTextures(ObjectModel* currentModel){
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXCreateTextureFromFile(device, currentModel->GetTextureFilename().c_str(), &texture);
	currentModel->SetTexture(texture);
}


/**
 * Release DirectX
 */
GraphicsSystem::~GraphicsSystem(){
	for(unsigned int i = 0; i < modelVec.size(); i++){
		(*modelVec[i]->GetTriVB())->Release();
		(*modelVec[i]->GetQuadVB())->Release();
	}
	device->Release();
	deviceCon->Release();
}

/**
 * Update loop
 */
void GraphicsSystem::Update(){
	//Clear the screen
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	//Begin drawing the scene
	device->BeginScene();
	
	//Draw all objects on the gameObjList
	ObjectModelMap::iterator mapIter;
	std::list<GameObject*>::iterator listIter;
	for(listIter = gameObjList.begin(); listIter != gameObjList.end(); ++listIter){
		//Retrieve the game object's desired model from the hash map
		mapIter = modelMap.find((*listIter)->GetModelName());
		if(mapIter != modelMap.end())
			//Perform the model's draw, using the game object for transformations
			(mapIter->second)->Draw(device, *listIter);
		else
			printf("Model '%s' to be drawn not found in hash map. Skipping this model's draw...", (*listIter)->GetModelName());
	}

	//End drawing the scene
	device->EndScene();

	device->Present(NULL, NULL, NULL, NULL);
}

/**
 * Set up the world view and camera
 */
void GraphicsSystem::SetupWorldProj(){
	//Set the position of the camera.
	float cameraX = 0;
	float cameraY = 0;
	float cameraH = -10.0f;

	//The eye point is the location of the viewer
	D3DXVECTOR3 eyePoint( cameraX, cameraY, cameraH );
	//The look-at point is where the viewer is looking
	D3DXVECTOR3 lookAtPoint( cameraX, cameraY, 0.0f );
	//The up vector defines which way is up
	D3DXVECTOR3 upVector( 0.0f, 1.0f, 0.0f );
	//Create a left-handed view matrix.
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &eyePoint, &lookAtPoint, &upVector);
	//Store the view matrix
	viewMatrix = matView;

	//Set the view matrix
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
}


///////////////////////////////////////////
//GETTERS AND SETTERS
//////////////////////////////////////////
int GraphicsSystem::GetViewportHeight(){
	return viewportHeight;
}

int GraphicsSystem::GetViewportWidth(){
	return viewportWidth;
}