/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LineSegment2D.c
Purpose: Implementation of Line Segment functions
Language: C
Author: Spencer Tracy, s.tracy, 60002313
- End Header -----------------------------------------------------*/

#include "LineSegment2D.h"

/*
This function builds a 2D line segment's data using 2 points
 - Computes the normal (Unit Vector)
 - Computes the dot product of the normal with one of the points

 - Parameters
	- LS:		The to-be-built line segment
	- Point0:	One point on the line
	- Point1:	Another point on the line

 - Returns 1 if the line equation was built successfully 
*/
int BuildLineSegment2D(LineSegment2D *LS, Vector2D *Point0, Vector2D *Point1)
{
	Vector2D outwardNormVec;
	Vector2DSet(&outwardNormVec, Point1->y - Point0->y, -(Point1->x - Point0->x));
	Vector2DNormalize(&outwardNormVec, &outwardNormVec);
	LS->mN = outwardNormVec;
	LS->mP0 = *Point0;
	LS->mP1 = *Point1;
	LS->mNdotP0 = Vector2DDotProduct(&outwardNormVec, Point0);
	return 1;
}