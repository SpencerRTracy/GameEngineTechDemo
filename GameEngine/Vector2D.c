/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.c
Purpose: Implementation for a 2D Vector Library
Language: C
Author: Spencer Tracy, s.tracy, 60002313
Creation date: 09/15/2013
- End Header -----------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.0001
#define  M_PI 3.14159265358979f

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = (pVec0->x)*(-1);
	pResult->y = (pVec0->y)*(-1);
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x) + (pVec1->x);
	pResult->y = (pVec0->y) + (pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x) - (pVec1->x);
	pResult->y = (pVec0->y) - (pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float vec0Length = (Vector2DLength(pVec0));
	pResult->x = (pVec0->x)/vec0Length;
	pResult->y = (pVec0->y)/vec0Length;

}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	Vector2D tempVec;
	Vector2DScale(&tempVec, pVec0, c);
	Vector2DAdd(pResult, &tempVec, pVec1);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
	pResult->x -= pVec1->x;
	pResult->y -= pVec1->y;
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
	return sqrt(Vector2DSquareLength(pVec0));
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	return ((pVec0->x) * (pVec0->x)) + ((pVec0->y) * (pVec0->y));
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return sqrt(Vector2DSquareDistance(pVec0, pVec1));
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return ((pVec0->x - pVec1->x) * (pVec0->x - pVec1->x)) 
		+ ((pVec0->y - pVec1->y) * (pVec0->y - pVec1->y));
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	//Creates a vector that has the given degree angle
	pResult->x = cos(angle/(180/M_PI));
	pResult->y = sin(angle/(180/M_PI));
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	//Creates a vector that has the given radian angle
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------
