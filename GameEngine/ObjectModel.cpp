#include "ObjectModel.h"

#include <iostream>


/**
 * Class to hold the model for any game object.
 * Multiple instances of game objects draw from the same model.
 */
ObjectModel::ObjectModel() : triangleVB(NULL), quadVB(NULL), textureFilename("NULL"), type(2)
{

}

/**
 * Draws the model to the screen using the given GameObject's
 * instance information (position, rotation, etc.)
 */
void ObjectModel::Draw(IDirect3DDevice9* device, GameObject* obj){
	//Multiplier to convert game coordinates to screen coordinates
	float scaler = 5.0f/70.0f;

	//Creating necessary matrices for transformation
	D3DXMATRIX matRotationX, matRotationY, matRotationZ, matScale, matTranslation;

	D3DXMatrixRotationY(&matRotationY, obj->GetRotY());
	D3DXMatrixRotationX(&matRotationX, obj->GetRotX());
	D3DXMatrixRotationZ(&matRotationZ, obj->GetRotZ());
	D3DXMatrixScaling(&matScale, obj->GetScaleX(), obj->GetScaleY(), (obj->GetScaleX() + obj->GetScaleY())/2.0f);
	D3DXMatrixTranslation(&matTranslation, obj->GetXPos() * scaler, obj->GetYPos() * scaler, obj->GetLayer());
	
	//Set the transformation matrix
	device->SetTransform(D3DTS_WORLD, &(matScale * matRotationX * matRotationY * matRotationZ * matTranslation));
	
	//Type denotes Font or Primitive type
	if(type == 2){
		//Type is a Primitive
		if(textureFilename != "NULL"){
			//Set the texture
			device->SetTexture(0, texture);
		} else{
			//This model has no texture.
			//Remove the last texture used.
			device->SetTexture(0, NULL);
		}

		//Check if there are triangles in the model.
		if(triangleCount > 0){
			//Set the stream source to the model's trianglevertex buffer
			device->SetStreamSource(0, triangleVB, 0, sizeof(Vertex));
			//Draw
			device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
		}

		//Check if there are quads in the model
		if(quadCount > 0){
			//Set the stream source to the model's quad vertex buffer
			device->SetStreamSource(0, quadVB, 0, sizeof(Vertex));
			//Draw
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, quadCount * 2);
		}
	} else if(type == 0 || type == 1){
		//The type is a font type

		//Set the font draw position
		rect.left = (long) obj->GetXPos() + width/2;
		rect.top = (long) (obj->GetYPos()*-1.0f) + height/2;

		//Draw
		font->DrawText(NULL, obj->GetText().c_str(), -1, &rect, DT_LEFT, 0xffffffff);
	}
}


//////////////////////////////////////////////
//Graphics Specific Getters and Setters
///////////////////////////////////////////////
void ObjectModel::SetTriangleCoords(Vertex* newCoords, int count, UINT byteSize){
	int arraySize = count * 3;
	triangleCoords = new Vertex[arraySize];
	for(int i = 0; i < arraySize; i++){
		triangleCoords[i] = newCoords[i];
	}
	triangleCount = count;
	triByteSize = byteSize;
}

void ObjectModel::SetQuadCoords(Vertex* newCoords, int count, UINT byteSize){
	//std::copy(std::begin(newCoords), std::end(newCoords), std::begin(quadCoords));
	int arraySize = count * 4;
	quadCoords = new Vertex[arraySize];
	for(int i = 0; i < arraySize; i++){
		quadCoords[i] = newCoords[i];
	}
	quadCount = count;
	quadByteSize = byteSize;
}

Vertex* ObjectModel::GetTriangleCoords(){
	return triangleCoords;
}

int ObjectModel::GetTriangleCount(){
	return triangleCount;
}

Vertex* ObjectModel::GetQuadCoords(){
	return quadCoords;
}

int ObjectModel::GetQuadCount(){
	return quadCount;
}

LPDIRECT3DVERTEXBUFFER9* ObjectModel::GetTriVB(){
	return &triangleVB;
}

LPDIRECT3DVERTEXBUFFER9* ObjectModel::GetQuadVB(){
	return &quadVB;
}

UINT ObjectModel::GetTriByteSize(){
	return triByteSize;
}

UINT ObjectModel::GetQuadByteSize(){
	return quadByteSize;
}

void ObjectModel::SetTextureFilename(std::string newFilename){
	textureFilename = newFilename;
}

std::string ObjectModel::GetTextureFilename(){
	return textureFilename;
}

void ObjectModel::SetTexture(LPDIRECT3DTEXTURE9 newTexture){
	texture = newTexture;
}

LPDIRECT3DTEXTURE9 ObjectModel::GetTexture(){
	return texture;
}

void ObjectModel::SetType(int newType){
	type = newType;
}

int ObjectModel::GetType(){
	return type;
}

void ObjectModel::SetRect(RECT newRect){
	rect = newRect;
}
RECT ObjectModel::GetRect(){
	return rect;
}

void ObjectModel::SetFont(LPD3DXFONT newFont){
	font = newFont;
}

LPD3DXFONT ObjectModel::GetFont(){
	return font;
}

void ObjectModel::SetWidth(int newWidth){
	width = newWidth;
}

void ObjectModel::SetHeight(int newHeight){
	height = newHeight;
}