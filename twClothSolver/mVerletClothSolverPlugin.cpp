

//MVerletClothSolverPlugin Implementation
//-----------------------------------------------
/*
Exported functions that maya uses to register the plugin 
*/

//Preprocessor
//-----------------------------------------------
#define EXPORT __declspec(dllexport)


//include
//-----------------------------------------------
#include <maya/MFnPlugin.h>
//clothSolver
#include "mVerletClothSolver.h"
#include "vcsPositionConstraintCmd.h"
#include "vcsCollisionConstraintSphereCmd.h"

//test
#include "tesselationVisualization.h"



//initialize
//-----------------------------------------------
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj, "Timm Wagener", "0.1", "2012");

	//clothSolver
	status = fnPlugin.registerCommand("vcsPositionConstraint", VcsPositionConstraintCmd::create);
	status = fnPlugin.registerCommand("vcsCollisionConstraintSphere", VcsCollisionConstraintSphereCmd::create);
	status = fnPlugin.registerNode(MVerletClothSolver::typeName, MVerletClothSolver::id, MVerletClothSolver::create, MVerletClothSolver::initialize, MPxNode::kLocatorNode);
	//test
	status = fnPlugin.registerNode(TesselationVisualization::typeName, TesselationVisualization::id, TesselationVisualization::create, TesselationVisualization::initialize, MPxNode::kLocatorNode);

	return MStatus::kSuccess;
}


//uninitialize
//-----------------------------------------------
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj);

	//clothSolver
	status = fnPlugin.deregisterCommand("vcsPositionConstraint");
	status = fnPlugin.deregisterCommand("vcsCollisionConstraintSphere");
	status = fnPlugin.deregisterNode(MVerletClothSolver::id);
	//test
	status = fnPlugin.deregisterNode(TesselationVisualization::id);

	return MStatus::kSuccess;
}
