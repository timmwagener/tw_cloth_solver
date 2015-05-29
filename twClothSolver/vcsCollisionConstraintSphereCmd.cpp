



//VcsCollisionConstraintSphereCmd Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "vcsCollisionConstraintSphereCmd.h"


//VcsCollisionConstraintSphereCmd
//-----------------------------------------------

//Attrs

//Methods
VcsCollisionConstraintSphereCmd::VcsCollisionConstraintSphereCmd():verbose(1){};
VcsCollisionConstraintSphereCmd::~VcsCollisionConstraintSphereCmd(){};

//create
void* VcsCollisionConstraintSphereCmd::create()
{
	return new VcsCollisionConstraintSphereCmd();
};

//doIt
MStatus VcsCollisionConstraintSphereCmd::doIt(const MArgList &args)
{
	//Get active selectionList
	//-----------------------------------------------

	//status
	MStatus status;

	//get selectionList
	MSelectionList selectionList;
	MGlobal::getActiveSelectionList(selectionList);

	//if !selectionList return
	if(!(selectionList.length() == 2))
	{
		if(verbose){MGlobal::displayInfo("Empty SelectionList or wrong number of selected objects. Please select 1 PolySphere and 1 clothSolver.");};
		return MS::kSuccess;
	};

	
	
	//Iterate active selectionList and get objects
	//-----------------------------------------------
	
	//itSelectionList
	MItSelectionList itSelectionList(selectionList);

	//MObjects
	MObject oCollisionSphereShape;
	MObject oClothSolver;
	MObject oTest;
	

	//iterate selectionList and get sphere
	while(!itSelectionList.isDone())
	{
		//get current object
		itSelectionList.getDependNode(oTest);

		//check if obj has fn of type transform
		//-----------------------------------------------
		
		if(oTest.hasFn(MFn::kTransform))
		{
			//Check if object has child of type mesh
			
			//MFnDagNode
			MFnDagNode fnDagNodeTest(oTest);
			MObject oTestShape = fnDagNodeTest.child(0);

			if(oTestShape.hasFn(MFn::kMesh))
			{
				//assign to oCollisionSphereShape
				oCollisionSphereShape = MObject(oTestShape);
			};
		}
		

		//check if obj is of type clothSolver
		//-----------------------------------------------
		
		else
		{
			//Get fnDepNode for object
			MFnDependencyNode fnDepNodeTest(oTest);

			//if name == clothSolver then assign
			if(fnDepNodeTest.typeName() == "verletClothSolver"){oClothSolver = MObject(oTest);};
		};
		
		//advance
		itSelectionList.next();
	};

	//Check if assignments worked correctly

	//create fnDepNode sets
	MFnDependencyNode fnDepNodeCollisionSphereShape(oCollisionSphereShape);
	MFnDependencyNode fnDepNodeClothSolver(oClothSolver);

	if(fnDepNodeClothSolver.typeName() != "verletClothSolver" || fnDepNodeCollisionSphereShape.typeName() != "mesh")
	{
		if(verbose){MGlobal::displayInfo("Selection for clothSolver or PolySphere was not correct.");};
		return MS::kSuccess;
	};



	//Connect sphereShape to clothSolver
	//-----------------------------------------------

	//Get Plugs to clothSolverAttrs
	//-----------------------------------------------
			
	//Get plug to compound array of clothSolver
	MPlug pCompoundArrayAttr = fnDepNodeClothSolver.findPlug("collisionConstraint");
			
	//lastArrayElementIndex
	int lastArrayElementIndex = pCompoundArrayAttr.numElements();
			
			
	//plug to last array element
	MPlug pCompoundAttr = pCompoundArrayAttr.elementByLogicalIndex(lastArrayElementIndex);

	//Get Plugs to children
	MPlug pTargetConstraintGeoMatrix = pCompoundAttr.child(2);
	MPlug pTargetConstraintGeo = pCompoundAttr.child(3);



	//Get Plugs to collisionSphereAttrs
	//-----------------------------------------------
	
	//Matrix
	//Get plug to compound array of collisionSphere
	MPlug pArrayAttr = fnDepNodeCollisionSphereShape.findPlug("parentMatrix");
			
	//GeoMatrix
	MPlug pSourceConstraintGeoMatrix = pArrayAttr.elementByLogicalIndex(0);

	//Geo
	MPlug pSourceConstraintGeo = fnDepNodeCollisionSphereShape.findPlug("outMesh");
			
	
	

	//Connect source to target
	//-----------------------------------------------

	//connect position plugs
	MDGModifier dgModifier;
	dgModifier.connect(pSourceConstraintGeoMatrix, pTargetConstraintGeoMatrix);
	dgModifier.connect(pSourceConstraintGeo, pTargetConstraintGeo);
	dgModifier.doIt();
	
	
	return MS::kSuccess;
};