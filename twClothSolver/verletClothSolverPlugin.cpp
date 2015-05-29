

//VerletClothSolverPlugin Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "verletClothSolverPlugin.h"


//VerletClothSolverPlugin 
//-----------------------------------------------

//Attributes

//Methods
VerletClothSolverPlugin::VerletClothSolverPlugin()
	:solver(0)//Initialisation list for member vars
{};

VerletClothSolverPlugin::~VerletClothSolverPlugin(){};

//solve
void VerletClothSolverPlugin::solve(solver_t solverType)
{
	
	
	//init?
	//-----------------------------------------------
	if(!solver)
	{
		//create new solver
		initSolver(solverType);
	}
	else if(solverType != solver->getType())
	{
		//delete old solver
		delete solver;
		//create new solver
		initSolver(solverType);
	}

	//solve
	//-----------------------------------------------
	//CPU
	if(solver->getType() == 1)
		solver->solve(staticSolverData, dynamicSolverData);
	//GPU
	else if(solver->getType() == 2)
		solver->solve(staticSolverDataGPU, dynamicSolverDataGPU);
};


//initSolver
void VerletClothSolverPlugin::initSolver(solver_t solverType)
{
	//create solver
	switch(solverType)
	{
		case GPU:
			//If cuda available create GPU
			if(VerletClothSolverGPU::cudaAvailable())
			{
				solver = new VerletClothSolverGPU();
				break;
			}
			else
			{
				//if not CPU version
				solver = new VerletClothSolverCPU();
				break;
			}
		case CPU:
			//if not CPU version
			solver = new VerletClothSolverCPU();
			break;
		default:
			break;
	}
};


//set_static_solver_data_gpu
void VerletClothSolverPlugin::set_static_solver_data_gpu() 
{
	//cuda
	cuda::set_static_solver_data_gpu(staticSolverDataGPU, staticSolverData);
};

//set_dynamic_solver_data_gpu
void VerletClothSolverPlugin::set_dynamic_solver_data_gpu() 
{
	//cuda
	cuda::set_dynamic_solver_data_gpu(dynamicSolverDataGPU, dynamicSolverData, staticSolverData);
};

//set_static_solver_data_from_gpu
void VerletClothSolverPlugin::set_static_solver_data_from_gpu()
{
	//cuda
	cuda::set_static_solver_data_from_gpu(staticSolverData, staticSolverDataGPU);
};

//is_static_solver_data_initialized
bool VerletClothSolverPlugin::is_static_solver_data_initialized()
{
	//check pVertexPositionList
	if(staticSolverData.pVertexPositionList)
		return true;
	return false;
};

//TMP
//-----------------------------------------------