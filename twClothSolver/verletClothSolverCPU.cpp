

//VerletClothSolverCPU Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolverCPU.h"


//VerletClothSolverCPU
//-----------------------------------------------

//Attributes


//Methods
VerletClothSolverCPU::VerletClothSolverCPU(){};
VerletClothSolverCPU::~VerletClothSolverCPU(){};


//solve
void VerletClothSolverCPU::solve(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	verletIntegration(staticSolverData, dynamicSolverData);
	for(int index = 1; index <= dynamicSolverData.repetitions; index++)
	{
		satisfyConstraints(staticSolverData, dynamicSolverData);
	};
};

//verletIntegration
void VerletClothSolverCPU::verletIntegration(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	#pragma omp parallel for if(dynamicSolverData.omp)
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		//Get vars for integration
		vcsVector vecNew;
		vcsVector vecPos = staticSolverData.pVertexPositionList[index];
		vcsVector vecOldpos = staticSolverData.pVertexOldpositionList[index];
		vcsVector vecForce = dynamicSolverData.pVertexForceList[index];
		float drag = 1.0 - dynamicSolverData.drag;

		//integration
		vecNew = (vecPos + ((vecPos - vecOldpos) * drag)) + vecForce;

		//update solverData
		staticSolverData.pVertexOldpositionList[index] = vecPos;
		staticSolverData.pVertexPositionList[index] = vecNew;
	}
	//std::cout<<"verletIntegrationCPU"<<std::endl;
};

//satisfyConstraints
void VerletClothSolverCPU::satisfyConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//constraint hierarchy
	collisionConstraints(staticSolverData, dynamicSolverData);
	stickConstraint(staticSolverData, dynamicSolverData);
	positionConstraints(staticSolverData, dynamicSolverData);
	
	//std::cout<<"satisfyConstraintsCPU"<<std::endl;
};

//collisionConstraints
void VerletClothSolverCPU::collisionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//CollisionGroundplane
	if(dynamicSolverData.collisionGroundplaneActive){collisionConstraintGroundplane(staticSolverData, dynamicSolverData);};
	//CollisionSpheres
	collisionConstraintSpheres(staticSolverData, dynamicSolverData);
	//CollisionConstraintConvex
	collisionConstraintConvex(staticSolverData, dynamicSolverData);
	//std::cout<<"CollisionConstraintsCPU"<<std::endl;
};

//collisionConstraintGroundplane
void VerletClothSolverCPU::collisionConstraintGroundplane(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	#pragma omp parallel for if(dynamicSolverData.omp)
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		if(staticSolverData.pVertexPositionList[index].y < dynamicSolverData.groundplaneHeight){staticSolverData.pVertexPositionList[index].y = dynamicSolverData.groundplaneHeight;};
	}
	//std::cout<<"CollisionConstraintGroundplaneCPU"<<std::endl;
};

//collisionConstraintSpheres
void VerletClothSolverCPU::collisionConstraintSpheres(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//iterate dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList
	for(int collisionSphereIndex = 0; collisionSphereIndex < dynamicSolverData.collisionConstraintSpheresCount * 2; collisionSphereIndex+=2 )
	{
		//vecDownToUp and radiusSphere
		vcsVector vecUp = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex];
		vcsVector vecDown = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex+1];
		vcsVector vecDownToUp = vecUp - vecDown;
		vcsVector vecSphereCenter = vecDown + (vecDownToUp * 0.5);
		float radiusSphere = (vecDownToUp * 0.5).length();

		//iterate each vertex in vertexPositionList and perform collision projection
		#pragma omp parallel for if(dynamicSolverData.omp)
		for(int index = 0; index < staticSolverData.vertexCount; index++)
		{
			//get current vcsVector
			vcsVector vecCurrentPoint = staticSolverData.pVertexPositionList[index];
			//get vector sphereCenter to currentPoint and its length
			vcsVector vecSphereCenterToCurrentPoint = vecCurrentPoint - vecSphereCenter;
			float vecSphereCenterToCurrentPointLength = vecSphereCenterToCurrentPoint.length();

			//check if length < radius
			if(vecSphereCenterToCurrentPointLength < radiusSphere)
			{
				//project currentPoint outward
				vcsVector currentPointProjected = vecSphereCenter + (vecSphereCenterToCurrentPoint.normal() * radiusSphere);
				//set to solverData
				staticSolverData.pVertexPositionList[index] = currentPointProjected;
			};
		};

	};
	
	//std::cout<<"CollisionConstraintSpheresCPU"<<std::endl;
};


//collisionConstraintConvex
void VerletClothSolverCPU::collisionConstraintConvex(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//triangleCount
	int triangleCount = 0;
	for(int index = 0; index < dynamicSolverData.collisionConstraintConvexCount; index++)
	{
		triangleCount += dynamicSolverData.pCollisionConstraintConvexTriangleCountList[index];
	};

	//triangleVertexCount
	int triangleVertexCount = triangleCount * 3;
	
	
	//For each point in vertexPositionList
	#pragma omp parallel for if(dynamicSolverData.omp)
	for(int vertexIndex = 0; vertexIndex < staticSolverData.vertexCount; vertexIndex++)
	{
		//For each triangle in collisionConvexGeo
		for(int triangleVertexIndex = 0; triangleVertexIndex < triangleVertexCount;  triangleVertexIndex += 3)
		{
			//std::cout<<"Triangle Index: "<<triangleVertexIndex/3<<std::endl;
			
			//Get vecNormal and planeDistance
			//-----------------------------------------------
			
			//Get triangle vertices
			vcsVector triangleVertex1 = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex];
			vcsVector triangleVertex2 = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex+1];
			vcsVector triangleVertex3 = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex+2];

			//get triangle edges
			vcsVector vecTriangleEdge1 = triangleVertex2 - triangleVertex1;
			vcsVector vecTriangleEdge2 = triangleVertex3 - triangleVertex2;

			//crossProduct
			vcsVector crossProductNormalized = vecTriangleEdge1.cross(vecTriangleEdge2).normal();

			//planeDistance
			float planeDistance = -1.0 * (triangleVertex1.dot(crossProductNormalized));
			
			
			//CollisionCheckValue for vertexPositions
			//-----------------------------------------------
			
			//vertexPosition
			vcsVector vertexPosition = staticSolverData.pVertexPositionList[vertexIndex];
			float collisionCheckValueVertexPosition = vertexPosition.dot(crossProductNormalized) + planeDistance;
			//vertexOldposition
			vcsVector vertexOldposition = staticSolverData.pVertexOldpositionList[vertexIndex];
			float collisionCheckValueVertexOldposition = vertexOldposition.dot(crossProductNormalized) + planeDistance;

			//check if both are behind or in front
			if(!(collisionCheckValueVertexPosition < 0 && collisionCheckValueVertexOldposition > 0))
			{
				//std::cout<<"No collision occured for triangle: "<<triangleVertexIndex/3<<std::endl;
				continue;
			}
			

			
			//Ray intersection with hyperplane
			//-----------------------------------------------

			//vecRay
			vcsVector vecRay = vertexPosition - vertexOldposition;

			//intersectionMultiplier
			float intersectionMultiplier = (-1.0 * (vertexOldposition.dot(crossProductNormalized) + planeDistance)) / 
											vecRay.dot(crossProductNormalized);

			//vecIntersection
			vcsVector vecIntersection = vertexOldposition + (vecRay * intersectionMultiplier);


			//Check if intersection is on triangle
			//-----------------------------------------------

			//vecIntersectionToVertices
			vcsVector vecIntersectionToTriangleVertex1 = triangleVertex1 - vecIntersection;
			vcsVector vecIntersectionToTriangleVertex2 = triangleVertex2 - vecIntersection;
			vcsVector vecIntersectionToTriangleVertex3 = triangleVertex3 - vecIntersection;

			//angles
			float angle1 = vecIntersectionToTriangleVertex1.angle(vecIntersectionToTriangleVertex2);
			float angle2 = vecIntersectionToTriangleVertex2.angle(vecIntersectionToTriangleVertex3);
			float angle3 = vecIntersectionToTriangleVertex3.angle(vecIntersectionToTriangleVertex1);

			//sumAngles
			float sumAngles = angle1 + angle2 + angle3;

			//std::cout<<"Sum Angles: " <<sumAngles <<std::endl;

			//check with margin
			float margin = 0.1;
			if(sumAngles > 360.0 - margin && sumAngles < 360.0 + margin)
			{
				//vecOffset
				vcsVector vecOffset = ((vertexOldposition - vecIntersection).normal()) * dynamicSolverData.collisionConstraintConvexOffset;
				
				//set position
				staticSolverData.pVertexPositionList[vertexIndex] = vecIntersection + vecOffset;
			};
			
			
		};
	};
	
	
	//std::cout<<"CollisionConstraintConvexCPU"<<std::endl;
};

//stickConstraint
void VerletClothSolverCPU::stickConstraint(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//iterate edges
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//restLength
		float restLength = staticSolverData.pEdgeRestlengthList[index];
		//vertexIndices
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);
		//vcsVector for indices
		vcsVector vec1 = staticSolverData.pVertexPositionList[vertexIndex1];
		vcsVector vec2 = staticSolverData.pVertexPositionList[vertexIndex2];
		//Compute new vector positions
		vcsVector delta = vec2 - vec1;
		float deltaLength = delta.length();
		float difference = (deltaLength - restLength) / deltaLength;
		vec1 = vec1 + delta * 0.5 * difference;
		vec2 = vec2 - delta * 0.5 * difference;

		//set vertexPositionList
		staticSolverData.pVertexPositionList[vertexIndex1] = vec1;
		staticSolverData.pVertexPositionList[vertexIndex2] = vec2;
	};
	//std::cout<<"stickConstraintCPU"<<std::endl;
};


//positionConstraint
void VerletClothSolverCPU::positionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{	
	//check if posCons 
	if(dynamicSolverData.positionConstraintCount)
	{
		//iterate posCons
		for(int index = 0; index < dynamicSolverData.positionConstraintCount; index++)
		{
			//check if posCon active
			if(dynamicSolverData.pPositionConstraintActiveList[index])
			{
				//check if vertexIndex in vertexCount
				if(dynamicSolverData.pPositionConstraintVertexIndexList[index] < staticSolverData.vertexCount)
				{
					//set vertexPositionList
					staticSolverData.pVertexPositionList[dynamicSolverData.pPositionConstraintVertexIndexList[index]] = dynamicSolverData.pPositionConstraintCoordinateList[index];
				}
			}
		}
	}

	//std::cout<<"positionConstraintsCPU"<<std::endl;
};