

#ifndef VERLETCLOTHSOLVERTYPES_H
#define VERLETCLOTHSOLVERTYPES_H

//include
//-----------------------------------------------

#include "verletClothSolverVector.h"


//typedef for VerletClothSolverVector
typedef VerletClothSolverVector vcsVector;

//declarations
//-----------------------------------------------

//staticSolverData_t 
//-----------------------------------------------
struct staticSolverData_t
{
	int vertexCount;
	int edgeCount;
	vcsVector* pVertexPositionList;
	vcsVector* pVertexOldpositionList;
	vcsVector* pVertexInitialpositionList;
	vcsVector* pEdgeVertexIndexList;
	float* pEdgeRestlengthList;
	//Methods
	staticSolverData_t():pVertexPositionList(0),pVertexOldpositionList(0),pVertexInitialpositionList(0),pEdgeVertexIndexList(0),pEdgeRestlengthList(0){};
	~staticSolverData_t(){};
};


//dynamicSolverData_t 
//-----------------------------------------------
struct dynamicSolverData_t
{
	//Attributes
	float drag;
	int collisionGroundplaneActive;
	float groundplaneHeight;
	vcsVector* pVertexForceList;
	int repetitions;
	int positionConstraintCount;
	int* pPositionConstraintActiveList;
	int* pPositionConstraintVertexIndexList;
	vcsVector* pPositionConstraintCoordinateList;
	int collisionConstraintSpheresCount;
	vcsVector* pCollisionConstraintSpheresVecUpVecDownList;
	int collisionConstraintConvexCount;
	int* pCollisionConstraintConvexTriangleCountList;
	int collisionConstraintConvexTriangleVertexCount;
	vcsVector* pCollisionConstraintConvexTriangleVertexPositionList;
	int thread_count;
	int omp;
	float collisionConstraintConvexOffset;
	//Methods
	dynamicSolverData_t():pPositionConstraintActiveList(0), pPositionConstraintVertexIndexList(0), pPositionConstraintCoordinateList(0), pCollisionConstraintSpheresVecUpVecDownList(0), pCollisionConstraintConvexTriangleCountList(0), pCollisionConstraintConvexTriangleVertexPositionList(0){};
	~dynamicSolverData_t(){};
};


//drawData_t 
//-----------------------------------------------
struct drawData_t
{
	//Attrs
	int collisionConstraintGroundPlaneVisible;
	float collisionConstraintGroundplaneDispSize;
	float collisionConstraintGroundplaneHeight;
	int drawMeshlinesActive;
	vcsVector* pVertexColorList;
	//Methods
	drawData_t():pVertexColorList(0){};
	~drawData_t(){};
};


//solver_t
//-----------------------------------------------
enum solver_t
{
	BASE = 0,
	CPU,
	GPU
};

//grid_size_t
//-----------------------------------------------
enum grid_size_t
{
	VERTEXCOUNT = 0,
	EDGECOUNT
};

#endif