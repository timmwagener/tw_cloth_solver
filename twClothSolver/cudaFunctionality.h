

#ifndef CUDAFUNCTIONALITY_H
#define CUDAFUNCTIONALITY_H

//include
//-----------------------------------------------
//std
#include <iostream>

//own
#include "verletClothSolverTypes.h"


//constants
//-----------------------------------------------




//declarations
//-----------------------------------------------
namespace cuda
{
	
	//cudaAvailable
	int cudaAvailable();
	//get_grid_size
	int get_grid_size(const staticSolverData_t&, const dynamicSolverData_t&, grid_size_t);
	//get_thread_count
	int get_thread_count(const dynamicSolverData_t&);

	//add_on_gpu
	float add_on_gpu(float, float);

	//set_static_solver_data_gpu
	void set_static_solver_data_gpu(staticSolverData_t&, const staticSolverData_t&);
	//set_dynamic_solver_data_gpu
	void set_dynamic_solver_data_gpu(dynamicSolverData_t&, const dynamicSolverData_t&, const staticSolverData_t&);
	//set_static_solver_data_from_gpu
	void set_static_solver_data_from_gpu(staticSolverData_t&, staticSolverData_t&);
	
	//verletIntegration
	void verletIntegration(staticSolverData_t&, dynamicSolverData_t&);
	//collisionConstraintGroundplane
	void collisionConstraintGroundplane(staticSolverData_t&, dynamicSolverData_t&);
	//collisionConstraintSpheres
	void collisionConstraintSpheres(staticSolverData_t&, dynamicSolverData_t&);
	//collisionConstraintConvex
	void collisionConstraintConvex(staticSolverData_t&, dynamicSolverData_t&);

	//stickConstraint
	void stickConstraint(staticSolverData_t&, dynamicSolverData_t&);

	//positionConstraints
	void positionConstraints(staticSolverData_t&, dynamicSolverData_t&);
	

	
};

#endif