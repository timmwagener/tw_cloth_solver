

#ifndef VERLETCLOTHSOLVERCPU_H
#define VERLETCLOTHSOLVERCPU_H


//VerletClothSolverCPU Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolver.h"


//VerletClothSolverCPU 
//-----------------------------------------------
class VerletClothSolverCPU: public VerletClothSolver
{
public:
	//Attributes
	//Methods
	VerletClothSolverCPU();
	~VerletClothSolverCPU();
	
	void solve(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	
	void verletIntegration(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	void satisfyConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	
	void collisionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	void collisionConstraintGroundplane(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	void collisionConstraintSpheres(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);
	void collisionConstraintConvex(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);

	void stickConstraint(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);

	void positionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData);

	solver_t getType(){return CPU;};
	

};



#endif