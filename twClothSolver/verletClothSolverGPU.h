

#ifndef VERLETCLOTHSOLVERGPU_CUH
#define VERLETCLOTHSOLVERGPU_CUH


//VerletClothSolverGPU Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolver.h"
#include "cudaFunctionality.h"

//constants
//-----------------------------------------------


//VerletClothSolverGPU 
//-----------------------------------------------
class VerletClothSolverGPU: public VerletClothSolver
{
public:
	//Attributes
	//Methods
	VerletClothSolverGPU();
	~VerletClothSolverGPU();
	
	void solve(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	
	void verletIntegration(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	void satisfyConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	
	void collisionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);

	solver_t getType(){return GPU;};
	
	//Cuda
	static int cudaAvailable();
	float add_on_gpu(float, float);

};



#endif