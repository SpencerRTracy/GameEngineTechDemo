#pragma once

#include "GraphicsSystem.h"
#include "GameObject.h"

/**
 * Vertex struct for DirectX to use for vertices
 */
struct Vertex{
	D3DXVECTOR3 position;
	DWORD color;
	float u;
	float v;
};


//Forward Declaration
class GameObject;

/**
 * Class to hold the model for any game object.
 * Multiple instances of game objects draw from the same model.
 */
class ObjectModel{
public:
	/**
	 * Class to hold the model for any game object.
	 * Multiple instances of game objects draw from the same model.
	 */
	ObjectModel();
	~ObjectModel(){}

	/////////////////////////////////
	//Graphics Specific Getters and Setters
	////////////////////////////////
	void SetTriangleCoords(Vertex* newCoords, int count, UINT byteSize);
	void SetQuadCoords(Vertex* newCoords, int count, UINT byteSize);
	Vertex* GetTriangleCoords();
	int GetTriangleCount();
	Vertex* GetQuadCoords();
	int GetQuadCount();
	UINT GetTriByteSize();
	UINT GetQuadByteSize();
	void SetTextureFilename(std::string newFilename);
	std::string GetTextureFilename();
	void SetTexture(LPDIRECT3DTEXTURE9 newTexture);
	LPDIRECT3DTEXTURE9 GetTexture();
	 
	void Draw(IDirect3DDevice9* device, GameObject* obj);
	void ConvertMatrix(D3DXMATRIX* n, Matrix2D* o);
	
	LPDIRECT3DVERTEXBUFFER9* GetTriVB();
	LPDIRECT3DVERTEXBUFFER9* GetQuadVB();

	void SetType(int newType);
	int GetType();

	void SetRect(RECT newRect);
	RECT GetRect();

	void SetFont(LPD3DXFONT newFont);
	LPD3DXFONT GetFont();

	void SetWidth(int width);
	void SetHeight(int height);

private:
	//Type (Font or Primitive)
	int type;

	//Vertex coordinates for any trianges in the model.
	Vertex* triangleCoords;

	//Count of the number of triangles in the model
	int triangleCount;

	//Size of all Tri data.
	UINT triByteSize;

	//Vertex coordinates for any quads in the model.
	Vertex* quadCoords;

	//Count of the number of quads in the model.
	int quadCount;

	//Size of all Quad data
	UINT quadByteSize;

	//Triangle vertex buffer
	LPDIRECT3DVERTEXBUFFER9 triangleVB;

	//Quad vertex buffer
	LPDIRECT3DVERTEXBUFFER9 quadVB;

	//Filename of the texture
	std::string textureFilename;

	//Texture type
	LPDIRECT3DTEXTURE9 texture;

	//Holds the font object
	LPD3DXFONT font;

	//Rect for positioning of font
	RECT rect;

	//Viewport width
	int width;

	//Viewport height
	int height;
};