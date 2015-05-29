

#ifndef VERLETCLOTHSOLVER_H
#define VERLETCLOTHSOLVER_H


//VerletClothSolver Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include <iostream>
#include <stdlib.h>
#include "verletClothSolverTypes.h"




//VerletClothSolver 
//-----------------------------------------------
class VerletClothSolver
{
public:
	//Attributes
	//Methods
	VerletClothSolver();
	~VerletClothSolver();
	
	virtual void solve(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData) = 0;

	virtual solver_t getType(){return BASE;};
	

};



#endif