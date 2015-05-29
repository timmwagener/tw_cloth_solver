

#ifndef VERLETCLOTHSOLVERVECTOR_H
#define VERLETCLOTHSOLVERVECTOR_H


//VerletClothSolverVector Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include <math.h>


//VerletClothSolverVector
//-----------------------------------------------
class VerletClothSolverVector
{
public:
	//Attributes
	float x,y,z,w;
	//Methods
	VerletClothSolverVector();
	VerletClothSolverVector(float aX, float aY, float aZ);
	~VerletClothSolverVector();
	
	VerletClothSolverVector operator+(VerletClothSolverVector rhsVec);
	VerletClothSolverVector operator-(VerletClothSolverVector rhsVec);
	VerletClothSolverVector operator*(const float &rhsScalar);
	VerletClothSolverVector operator/(const float &rhsScalar);
	bool operator==(VerletClothSolverVector &rhsVec);
	bool operator!=(VerletClothSolverVector &rhsVec);

	float length();
	VerletClothSolverVector normal();
	float angle(VerletClothSolverVector &rhsVec);
	float dot(VerletClothSolverVector &rhsVec);
	VerletClothSolverVector cross(VerletClothSolverVector &rhsVec);

};



#endif