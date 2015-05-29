

#ifndef VCSPOSITIONCONSTRAINTCMD_H
#define VCSPOSITIONCONSTRAINTCMD_H

//VcsPositionConstraintCmd Declaration
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



//VcsPositionConstraintCmd
//-----------------------------------------------
class VcsPositionConstraintCmd: public MPxCommand
{
public:
	//Attrs
	bool verbose;
	//Methods
	VcsPositionConstraintCmd();
	~VcsPositionConstraintCmd();
	static void* create();
	virtual MStatus doIt(const MArgList &args);
	
};



#endif