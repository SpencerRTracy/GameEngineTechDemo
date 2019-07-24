/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.c
Purpose: Implementation for a 2D 3x3 Matrix Library
Language: C
Author: Spencer Tracy, s.tracy, 60002313
Creation date: 09/15/2013
- End Header -----------------------------------------------------*/

#include "Matrix2D.h"

#define  M_PI 3.14159265358979f

/*
This function sets the matrix result to all zeros
*/
void Matrix2DZero(Matrix2D *pResult){
	int i;
	for(i = 0; i < 3; i++){
		int n;
		for(n = 0; n < 3; n++){
			//Create 0 Matrix
			pResult->m[i][n] = 0.0f;
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	int i;
	for(i = 0; i < 3; i++){
		int n;
		for(n = 0; n < 3; n++){
			if(i == n){
				pResult->m[i][n] = 1;
			} else{
				pResult->m[i][n] = 0;
			}
		}
	}
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	int i;
	for(i = 0; i < 3; i++){
		int n;
		for(n = 0; n < 3; n++){
			pResult->m[i][n] = pMtx->m[n][i];
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	int i;
	/*
	  Create a matrix to store multiplication results
	  Necessary in case pMtx0 or pMtx1 is same pointer as pResult
	*/
	Matrix2D resultMatrix;
	//Initialize (zero out) the newly created matrix
	Matrix2DZero(&resultMatrix);

	//Perform Matrix Multiplication
	for(i = 0; i < 3; i++){
		int n;
		for(n = 0; n < 3; n++){
			int p;
			for(p=0; p < 3; p++){
				resultMatrix.m[i][n] += (pMtx0->m[i][p] * pMtx1->m[p][n]);
			}
		}
	}

	//Can now modify pResult matrix to copy over the result
	for(i = 0; i < 3; i++){
		int n;
		for(n = 0; n < 3; n++){
			pResult->m[i][n] = resultMatrix.m[i][n];
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][2] = x;
	pResult->m[1][2] = y;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	float radianAngle;
	Matrix2DIdentity(pResult);
	radianAngle = Angle / (180/M_PI);
	pResult->m[0][0] = cos(radianAngle);
	pResult->m[0][1] = sin(radianAngle)*(-1);
	pResult->m[1][0] = sin(radianAngle);
	pResult->m[1][1] = cos(radianAngle);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cos(Angle);
	pResult->m[0][1] = sin(Angle)*(-1);
	pResult->m[1][0] = sin(Angle);
	pResult->m[1][1] = cos(Angle);
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	int i;

	//Create a 3x1 Homogenous Matrix representation
	float homogCoordVec[] = {pVec->x, pVec->y, 1};

	/*
	  Create a matrix to store multiplication results
	  Necessary in case pResult is same pointer as pVec
	*/
	float resultVec[] = {0, 0, 0};

	//Perform 3x3 x 3x1 Matrix Multiplication
	for(i = 0; i < 3; i++){
		int p;
		for(p=0; p < 3; p++){
			resultVec[i] += (pMtx->m[i][p] * homogCoordVec[p]);
		}
	}

	//Convert from Homogenous to standard non-weighted x,y coordinate points
	pResult->x = resultVec[0] / resultVec[2];
	pResult->y = resultVec[1] / resultVec[2];
}

// ---------------------------------------------------------------------------
