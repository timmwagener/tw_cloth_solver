

#ifndef VERLETCLOTHSOLVERPLUGIN_H
#define VERLETCLOTHSOLVERPLUGIN_H

//VerletClothSolverPlugin Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolverCPU.h"
#include "verletClothSolverGPU.h"



//VerletClothSolverPlugin 
//-----------------------------------------------
class VerletClothSolverPlugin
{
public:
	//Attributes
	staticSolverData_t staticSolverData;
	staticSolverData_t staticSolverDataGPU;
	dynamicSolverData_t dynamicSolverData;
	dynamicSolverData_t dynamicSolverDataGPU;
	
	//Methods
	VerletClothSolverPlugin();
	~VerletClothSolverPlugin();
	
	//VerletClothSolverPlugin
	void solve(solver_t solverType);
	void initSolver(solver_t solverType);
	solver_t getType(){return solver->getType();};
	
	//staticSolverData
	staticSolverData_t getStaticSolverData(){return staticSolverData;};
	virtual void setStaticSolverData(void* proprietaryData) = 0;
	void set_static_solver_data_gpu();
	void set_static_solver_data_from_gpu();
	bool is_static_solver_data_initialized();
	virtual void printStaticSolverData() = 0;
	
	//dynamicSolverData
	dynamicSolverData_t getDynamicSolverData(){return dynamicSolverData;};
	virtual void setDynamicSolverData(void* proprietaryData) = 0;
	void set_dynamic_solver_data_gpu();
	virtual void printDynamicSolverData() = 0;

	
	
private:
	//Attributes
	VerletClothSolver* solver;
	//Methods

};



#endif