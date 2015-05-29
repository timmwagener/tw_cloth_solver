

//VerletClothSolverVector Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolverVector.h"


//VerletClothSolver
//-----------------------------------------------

//Attributes


//Methods

//Default constructor
VerletClothSolverVector::VerletClothSolverVector()
	:x(0.0),y(0.0),z(0.0),w(1.0)
{};
//Explicit constructor
VerletClothSolverVector::VerletClothSolverVector(float aX, float aY, float aZ)
	:x(aX),y(aY),z(aZ),w(1.0)
{};

//Destructor
VerletClothSolverVector::~VerletClothSolverVector(){};

//Arithmetic Operator Overload: +
VerletClothSolverVector VerletClothSolverVector::operator+(VerletClothSolverVector rhsVec)
{
	float tempX = x + rhsVec.x;
	float tempY = y + rhsVec.y;
	float tempZ = z + rhsVec.z;
	return VerletClothSolverVector(tempX,tempY,tempZ);
};

//Arithmetic Operator Overload: -
VerletClothSolverVector VerletClothSolverVector::operator-(VerletClothSolverVector rhsVec)
{
	float tempX = x - rhsVec.x;
	float tempY = y - rhsVec.y;
	float tempZ = z - rhsVec.z;
	return VerletClothSolverVector(tempX,tempY,tempZ);
};

//Arithmetic Operator Overload: *
VerletClothSolverVector VerletClothSolverVector::operator*(const float &rhsScalar)
{
	float tempX = x * rhsScalar;
	float tempY = y * rhsScalar;
	float tempZ = z * rhsScalar;
	return VerletClothSolverVector(tempX,tempY,tempZ);
};

//Arithmetic Operator Overload: /
VerletClothSolverVector VerletClothSolverVector::operator/(const float &rhsScalar)
{
	float tempX = x / rhsScalar;
	float tempY = y / rhsScalar;
	float tempZ = z / rhsScalar;
	return VerletClothSolverVector(tempX,tempY,tempZ);
};

//Comparison Operator ==
bool VerletClothSolverVector::operator==(VerletClothSolverVector &rhsVec)
{
	return (x == rhsVec.x && y == rhsVec.y && z == rhsVec.z);
};

//Negative Comparison Operator
bool VerletClothSolverVector::operator!=(VerletClothSolverVector &rhsVec)
{
	return (x != rhsVec.x && y != rhsVec.y && z != rhsVec.z);
};

//length
float VerletClothSolverVector::length()
{
	float length = float(sqrt((x*x) + (y*y) +(z*z)));
	return length;
};

//normal
VerletClothSolverVector VerletClothSolverVector::normal()
{
	float length = float(sqrt((x*x) + (y*y) +(z*z)));
	return VerletClothSolverVector(x/length, y/length, z/length);
};

//angle
float VerletClothSolverVector::angle(VerletClothSolverVector &rhsVec)
{
	//dot product
	float dotProduct = dot(rhsVec);

	//rhsVecLength
	float rhsVecLength = rhsVec.length();

	//cosAngle
	float cosAngle = dotProduct / (length() * rhsVecLength);

	//angle
	float angle = acos(cosAngle) * (180.0 / 3.14159265);

	return angle;
};

//dot
float VerletClothSolverVector::dot(VerletClothSolverVector &rhsVec)
{
	//multiply x,y,z
	float multipliedX = x * rhsVec.x;
	float multipliedY = y * rhsVec.y;
	float multipliedZ = z * rhsVec.z;

	return (multipliedX+multipliedY+multipliedZ);

};

//cross
VerletClothSolverVector VerletClothSolverVector::cross(VerletClothSolverVector &rhsVec)
{
	//crossX
	float crossX = y*rhsVec.z - z*rhsVec.y;
	float crossY = z*rhsVec.x - x*rhsVec.z;
	float crossZ = x*rhsVec.y - y*rhsVec.x;

	return VerletClothSolverVector(crossX, crossY, crossZ);

};
