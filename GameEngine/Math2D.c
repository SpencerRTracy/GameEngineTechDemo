/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.c
Purpose: Implementation for a 2D Math Library
Language: C
Author: Spencer Tracy, s.tracy, 60002313
Creation date: 09/15/2013
- End Header -----------------------------------------------------*/

#include "Math2D.h"
#include "stdio.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	if(Vector2DSquareDistance(pP, pCenter) >= Radius*Radius){
		return 0;
	} else{
		return 1;
	}
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	if((pPos->x) < ((pRect->x) - (Width/2)) 
	  || (pPos->x) > ((pRect->x) + (Width/2)) 
	  || (pPos->y) < ((pRect->y) - (Height/2)) 
	  || (pPos->y) > ((pRect->y) + (Height/2)) )
	{
		return 0;
	} else {
		return 1;
	}
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	float radiusSumSq = (Radius0 + Radius1) * (Radius0 + Radius1);
	float centersDistanceSq = Vector2DSquareDistance(pCenter0, pCenter1);
	if(centersDistanceSq <= radiusSumSq){ 
		return 1; 
	} else {
		return 0;
	}
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	if( (pRect0->x) - (Width0/2) > (pRect1->x) + (Width1/2) ){
		return 0;
	} else if( (pRect1->x) - (Width1/2) > (pRect0->x) + (Width0/2)){
		return 0;
	} else if( (pRect0->y) + (Height0/2) < (pRect1->y) - (Height1/2)){
		return 0;
	} else if( (pRect1->y) + (Height1/2) < (pRect0->y) - (Height0/2)){
		return 0;
	} else {
		return 1;
	}
}

/*
This function checks if a static circle is intersecting with a static rectangle

Circle:		Center is "Center", radius is "Radius"
Rectangle:	Center is "Rect", width is "Width" and height is "Height"
Function returns true is the circle and rectangle are intersecting, otherwise it returns false
*/

int StaticCircleToStaticRectangle(Vector2D *pCenter, float Radius, Vector2D *pRect, float Width, float Height)
{
	float rectLeft	 = (pRect->x) - Width/2;
	float rectRight  = (pRect->x) + Width/2;
	float rectTop	 = (pRect->y) + Height/2;
	float rectBottom = (pRect->y) - Height/2;

	float closestX	 = (pCenter->x) < rectLeft ? rectLeft : ( (pCenter->x) > rectRight ? rectRight : pCenter->x );
	float closestY	 = (pCenter->y) < rectBottom ? rectBottom : ( (pCenter->y) > rectTop ? rectTop : pCenter->y );
	
	Vector2D closestPointToCircle;
	closestPointToCircle.x = closestX;
	closestPointToCircle.y = closestY;

	return StaticPointToStaticCircle(&closestPointToCircle, pCenter, Radius);
}

/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(Vector2D *P, LineSegment2D *LS){
	return Vector2DDotProduct(&(LS->mN), P) - LS->mNdotP0;
}


/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi){
	Vector2D velVec;
	/* Create commonly used dot products so as to not recompute them
	 * throughout this function
	 */
	float nDotPs = Vector2DDotProduct(&(LS->mN), Ps);
	float nDotPe = Vector2DDotProduct(&(LS->mN), Pe);
	float nDotV;
	/* Create V Vector for Pe and Ps */
	Vector2DSub(&velVec, Pe, Ps);
	/* Finish creating dot products that require V Vector */
	nDotV = Vector2DDotProduct(&(LS->mN), &velVec);
	/* Check if Pe and Ps lie on same half plane */
	if(((nDotPs < LS->mNdotP0) && (nDotPe < LS->mNdotP0)) || ((nDotPs > LS->mNdotP0) && (nDotPe > LS->mNdotP0)) || (nDotV == 0) ){
			/* No Collision */
			return -1.0f;
	} else {
		/* There may be a collision.  More tests needed */
		Vector2D intersectPoint;
		Vector2D tempSub1;
		Vector2D tempSub2;
		/* Compute time of supposed intersection */
		float intersectTime = (LS->mNdotP0 - nDotPs)/(nDotV);
		
		/*Testing if out of time bounds*/
		if(intersectTime > 1 || intersectTime < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Compute the point of supposed intersection */
		Vector2DScale(&velVec, &velVec, ( (LS->mNdotP0 - nDotPs) / nDotV ));
		Vector2DAdd(&intersectPoint, Ps, &velVec);

		/* Testing if out of bounds of line segment */
		Vector2DSub(&tempSub1, &intersectPoint, &(LS->mP0));
		Vector2DSub(&tempSub2, &(LS->mP1), &(LS->mP0));
		if(Vector2DDotProduct(&tempSub1, &tempSub2) < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Testing if out of bounds of line segment */
		Vector2DSub(&tempSub1, &intersectPoint, &(LS->mP1));
		Vector2DSub(&tempSub2, &(LS->mP0), &(LS->mP1));
		if(Vector2DDotProduct(&tempSub1, &tempSub2) < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Collision confirmed at this point.
		 * Set the point of intersection
		 */
		Vector2DSet(Pi, intersectPoint.x, intersectPoint.y);

		/* Return the intersection time of confirmed collision */
		return intersectTime;
	}
}


/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi){
	Vector2D velVec;
	/* Create commonly used dot products so as to not recompute them
	 * throughout this function
	 */
	float nDotPs = Vector2DDotProduct(&(LS->mN), Ps);
	float nDotPe = Vector2DDotProduct(&(LS->mN), Pe);
	float nDotV;
	/* Create V Vector for Pe and Ps */
	Vector2DSub(&velVec, Pe, Ps);
	/* Finish creating dot products that require V Vector */
	nDotV = Vector2DDotProduct(&(LS->mN), &velVec);
	
	/* Check if Pe and Ps lie on same half plane */
	if(((nDotPs - LS->mNdotP0 < -Radius) && (nDotPe - LS->mNdotP0 < -Radius)) || ((nDotPs - LS->mNdotP0 > Radius) && (nDotPe - LS->mNdotP0 > Radius)) ){
			/* No Collision */
			return -1.0f;
	} else {
		/* There may be a collision.  More tests needed */
		Vector2D intersectPoint;
		Vector2D tempSub1;
		Vector2D tempSub2;
		/* Compute time of supposed intersection */
		float intersectTimeA = (LS->mNdotP0 - nDotPs + Radius)/(nDotV);
		float intersectTimeB = (LS->mNdotP0 - nDotPs - Radius)/(nDotV);
		float intersectTime;
		if(intersectTimeA < intersectTimeB){
			intersectTime = intersectTimeA;
		} else {
			intersectTime = intersectTimeB;
		}

		/*Testing if out of time bounds*/
		if(intersectTime > 1 || intersectTime < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Compute the point of supposed intersection */
		Vector2DScale(&intersectPoint, &velVec, ( intersectTime));
		Vector2DAdd(&intersectPoint, Ps, &intersectPoint);

		/* Testing if out of bounds of line segment */
		Vector2DSub(&tempSub1, &intersectPoint, &(LS->mP0));
		Vector2DSub(&tempSub2, &(LS->mP1), &(LS->mP0));
		if(Vector2DDotProduct(&tempSub1, &tempSub2) < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Testing if out of bounds of line segment */
		Vector2DSub(&tempSub1, &intersectPoint, &(LS->mP1));
		Vector2DSub(&tempSub2, &(LS->mP0), &(LS->mP1));
		if(Vector2DDotProduct(&tempSub1, &tempSub2) < 0){
			/* No Collision */
			return -1.0f;
		}

		/* Collision confirmed at this point.
		 * Set the point of intersection
		 */
		Vector2DSet(Pi, intersectPoint.x, intersectPoint.y);

		/* Return the intersection time of confirmed collision */
		return intersectTime;
	}
}


/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi, Vector2D *R){
	/* Create vectors necessary to compute reflection vector */
	Vector2D iVec;
	Vector2D rVec;
	/* Check if a collision has occurred.
	 * Reflection not needed if no collision.
	 */
	float intersectionTime = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
	if(intersectionTime == -1.0f){
		/* No Collision */
		return intersectionTime;
	}
	/* Collision Confirmed */

	/* Compute i Vector */
	Vector2DSub(&iVec, Pe, Pi);

	/* Use i Vector to compute reflection vector */
	Vector2DScale(&rVec, &(LS->mN), 2 * Vector2DDotProduct(&iVec, &(LS->mN)));
	Vector2DSub(R, &iVec, &rVec);

	/* Return the intersection time of confirmed collision */
	return intersectionTime;
}


/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi, Vector2D *R){
	/* Create vectors necessary to compute reflection vector */
	Vector2D iVec;
	Vector2D rVec;
	/* Check if a collision has occurred.
	 * Reflection not needed if no collision.
	 */
	float intersectionTime = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);
	if(intersectionTime == -1.0f){
		/* No Collision */
		return intersectionTime;
	}

	/* Collision Confirmed */
	
	/* Compute i Vector */
	Vector2DSub(&iVec, Pe, Pi);

	/* Use i Vector to compute reflection vector */
	Vector2DScale(&rVec, &(LS->mN), 2 * Vector2DDotProduct(&iVec, &(LS->mN)));
	Vector2DSub(R, &iVec, &rVec);

	/* Return the intersection time of confirmed collision */
	return intersectionTime;
}


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi){
	float m;
	float s;
	float nSq;
	float ti0;
	float ti1;
	float tiFinal;
	float radius1Sq = Radius*Radius;
	Vector2D BsC;
	Vector2D velVec;
	Vector2D velVecNorm;
	Vector2D intersectPoint;
	Vector2DSub(&velVec, Pe, Ps);
	Vector2DSub(&BsC, Center, Ps);
	Vector2DNormalize(&velVecNorm, &velVec);
	m = Vector2DDotProduct(&BsC, &velVecNorm);
	if(m < 0 && StaticPointToStaticCircle(Ps, Center, Radius)){
		return -1.0f;
	}
	
	nSq = (Vector2DLength(&BsC)*Vector2DLength(&BsC)) - (m*m);

	if(nSq > radius1Sq){
		/* No Collision */
		return -1.0f;
	}
	
	s = sqrt(radius1Sq - nSq);
	ti0 = (m - s) / Vector2DLength(&velVec);
	ti1 = (m + s) / Vector2DLength(&velVec);
	if(ti0 < ti1){
		tiFinal = ti0;
	} else{
		tiFinal = ti1;
	}

	/*Testing if out of time bounds*/
	if(tiFinal > 1 || tiFinal < 0){
		/* No Collision */
		return -1.0f;
	}

	/* Compute the point of supposed intersection */
	Vector2DScale(&intersectPoint, &velVec, tiFinal);
	Vector2DAdd(Pi, Ps, &intersectPoint);

	return tiFinal;
}



/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi, Vector2D *R){
	float intersectionTime = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);
	Vector2D nVec;
	Vector2D mVec;
	Vector2D tempVec;
	
	if(intersectionTime == -1.0f){
		return intersectionTime;
	}

	Vector2DSub(&nVec, Pi, Center);
	Vector2DNormalize(&nVec, &nVec);

	Vector2DSub(&mVec, Ps, Pi);

	Vector2DScale(&tempVec, &nVec, ( 2*Vector2DDotProduct(&mVec, &nVec) ));
	Vector2DSub(R, &tempVec, &mVec);

	return intersectionTime;
}


/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi){
	float newRadiusForTest = Radius0 + Radius1;
	return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, newRadiusForTest, Pi);
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi, Vector2D *R){
	float intersectionTime = AnimatedCircleToStaticCircle(Center0s, Center0e, Radius0, Center1, Radius1, Pi);
	Vector2D nVec;
	Vector2D mVec;
	Vector2D tempVec;
	
	if(intersectionTime == -1.0f){
		return intersectionTime;
	}

	Vector2DSub(&nVec, Pi, Center1);
	Vector2DNormalize(&nVec, &nVec);

	Vector2DSub(&mVec, Center0s, Pi);

	Vector2DScale(&tempVec, &nVec, ( 2*Vector2DDotProduct(&mVec, &nVec) ));
	Vector2DSub(R, &tempVec, &mVec);

	return intersectionTime;
}
