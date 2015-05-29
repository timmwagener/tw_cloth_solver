

//VerletClothSolverGPU Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolverGPU.h"


//VerletClothSolverGPU
//-----------------------------------------------

//Attributes


//Methods
VerletClothSolverGPU::VerletClothSolverGPU(){};
VerletClothSolverGPU::~VerletClothSolverGPU(){};


//solve
void VerletClothSolverGPU::solve(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	verletIntegration(staticSolverData, dynamicSolverData);
	for(int index = 1; index <= dynamicSolverData.repetitions; index++)
	{
		satisfyConstraints(staticSolverData, dynamicSolverData);
	};
	
};

//verletIntegration
void VerletClothSolverGPU::verletIntegration(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//verletIntegration
	cuda::verletIntegration(staticSolverData, dynamicSolverData);
	
	//log
	//std::cout<<"verletIntegrationGPU"<<std::endl;
};

//satisfyConstraints
void VerletClothSolverGPU::satisfyConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//constraint hierarchy
	collisionConstraints(staticSolverData, dynamicSolverData);
	cuda::stickConstraint(staticSolverData, dynamicSolverData);
	cuda::positionConstraints(staticSolverData, dynamicSolverData);
	//std::cout<<"satisfyConstraintsGPU"<<std::endl;
};

//collisionConstraints
void VerletClothSolverGPU::collisionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//CollisionConstraintGroundplane
	if(dynamicSolverData.collisionGroundplaneActive)
		cuda::collisionConstraintGroundplane(staticSolverData, dynamicSolverData);
	//CollisionConstraintSpheres
	cuda::collisionConstraintSpheres(staticSolverData, dynamicSolverData);
	//CollisionConstraintConvex
	cuda::collisionConstraintConvex(staticSolverData, dynamicSolverData);
	//std::cout<<"CollisionConstraintsGPU"<<std::endl;
};

//cudaAvailable
int VerletClothSolverGPU::cudaAvailable()
{
	return cuda::cudaAvailable();
};

//add_on_gpu
float VerletClothSolverGPU::add_on_gpu(float a, float b)
{
	return cuda::add_on_gpu(a, b);
};