

#ifndef VCSCOLLISIONCONSTRAINTSPHERECMD_H
#define VCSCOLLISIONCONSTRAINTSPHERECMD_H

//VcsCollisionConstraintSphereCmd Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDagModifier.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>



//VcsCollisionConstraintSphereCmd
//-----------------------------------------------
class VcsCollisionConstraintSphereCmd: public MPxCommand
{
public:
	//Attrs
	bool verbose;
	//Methods
	VcsCollisionConstraintSphereCmd();
	~VcsCollisionConstraintSphereCmd();
	static void* create();
	virtual MStatus doIt(const MArgList &args);
	
};



#endif