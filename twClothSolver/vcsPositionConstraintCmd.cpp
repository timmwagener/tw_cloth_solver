

//VcsPositionConstraintCmd Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "vcsPositionConstraintCmd.h"


//VcsPositionConstraintCmd
//-----------------------------------------------

//Attrs

//Methods
VcsPositionConstraintCmd::VcsPositionConstraintCmd():verbose(1){};
VcsPositionConstraintCmd::~VcsPositionConstraintCmd(){};

//create
void* VcsPositionConstraintCmd::create()
{
	return new VcsPositionConstraintCmd();
};

//doIt
MStatus VcsPositionConstraintCmd::doIt(const MArgList &args)
{
	
	//Get active selectionList
	//-----------------------------------------------

	//status
	MStatus status;

	//get selectionList
	MSelectionList selectionList;
	MGlobal::getActiveSelectionList(selectionList);

	//if !selectionList return
	if(!selectionList.length())
	{
		if(verbose){MGlobal::displayInfo("Empty SelectionList. Please select at least one vertex of an object connected to a vcsClothSolver.");};
		return MS::kSuccess;
	};

	
	
	
	//Iterate active selectionList
	//-----------------------------------------------
	
	//dagPath and component
	MDagPath dagPath;
	MObject oComponent;
	//itSelectionList
	MItSelectionList itSelectionList(selectionList);

	//iterate selectionList (for each selected dagNode)
	while(!itSelectionList.isDone())
	{	
		
		//Get connected ClothSolver
		//-----------------------------------------------

		//set dagPath and oComponent
		itSelectionList.getDagPath(dagPath, oComponent);

		//fnDagNode
		MFnDagNode fnDagNode(dagPath);
		//MGlobal::displayInfo(fnDagNode.typeName());

		
		//pOutMesh
		MPlug pOutMesh = fnDagNode.findPlug("outMesh", true, &status);
		if(status == MStatus::kFailure)
		{
			if(verbose){MGlobal::displayInfo("Selected object does not have plug outMesh");};
			//advance
			itSelectionList.next();
			continue;
		};
		
		//pArrayOutMesh
		MPlugArray pArrayOutMesh;
		pOutMesh.connectedTo(pArrayOutMesh, false, true);

		//Check if plug is connected to anything
		if(!pArrayOutMesh.length())
		{
			if(verbose){MGlobal::displayInfo("Plug outMesh not connected to anything. Continuing.");};
			//advance
			itSelectionList.next();
			continue;
		};

		//oCurrentDestination
		MObject oCurrentDestination;

		//iterate pArrayOutMesh
		for(int index = 0; index < pArrayOutMesh.length(); index++)
		{
			//pDestination
			MPlug pDestination;
			pDestination = pArrayOutMesh[index];
			
			//fnDepNodeDestination
			MFnDependencyNode fnDepNodeDestination(pDestination.node());
			
			//check if typename ==verletClothSolver
			if(fnDepNodeDestination.typeName() == "verletClothSolver"){oCurrentDestination = fnDepNodeDestination.object();break;}
			else{oCurrentDestination = fnDepNodeDestination.object();};
		};

		//fnDepNodeClothSolver
		MFnDependencyNode fnDepNodeClothSolver(oCurrentDestination);
		//Check if object really clothSolver
		if(!(fnDepNodeClothSolver.typeName() == "verletClothSolver"))
		{
			if(verbose){MGlobal::displayInfo(MString("Selected object is not connected to a clothSolver: ") +fnDepNodeClothSolver.name());};
			//advance
			itSelectionList.next();
			continue;
		};
		
		//Display clothSolver Name and type
		if(verbose)
		{
			MGlobal::displayInfo(fnDepNodeClothSolver.name());
			MGlobal::displayInfo(fnDepNodeClothSolver.typeName());
			MGlobal::displayInfo("-----------------------------");
		};
		

		
		
		
		
		//Create posConstraintLists
		//-----------------------------------------------
		
		//create itVertices
		MItMeshVertex itVertices(dagPath, oComponent);

		//dataLists
		int verticesCount = itVertices.count();
		MPoint* posConstraintPositionList = new MPoint[verticesCount];
		int* posConstraintVertexIndexList = new int[verticesCount];
		MObject* posConstraintObjectList = new MObject[verticesCount];

		//index
		int runtimeVar = 0;
		
		//iterate itVertices
		while(!itVertices.isDone())
		{
			//Get ws pos
			MPoint wsPos = itVertices.position(MSpace::kWorld);
			//get vertexIndex
			int vertexIndex = itVertices.index();

			//create locator
			MObject oPositionConstraint;
			MDagModifier dagModifier;
			oPositionConstraint = dagModifier.createNode("locator", MObject::kNullObj );
			//rename locator
			MString posConstraintName = MString("vcsPositionConstraint");
			dagModifier.renameNode(oPositionConstraint, posConstraintName);
			dagModifier.doIt();

			//translate locator
			MFnTransform fnTransformLocator(oPositionConstraint);
			MVector vecPos = MVector(wsPos);
			fnTransformLocator.setTranslation(vecPos, MSpace::kTransform);

			//fill in arrays
			posConstraintPositionList[runtimeVar] = wsPos;
			posConstraintVertexIndexList[runtimeVar] = vertexIndex;
			posConstraintObjectList[runtimeVar] = oPositionConstraint;

			runtimeVar++;

			//advance
			itVertices.next();
		};



		
		//Print posConstraintLists
		//-----------------------------------------------

		//iterate arrays and print info
		if(verbose)
		{
			for(int index = 0; index < verticesCount; index++)
			{
				//getData
				MPoint wsPos = posConstraintPositionList[index];
				int vertexIndex = posConstraintVertexIndexList[index];
				MObject oPositionConstraint = posConstraintObjectList[index];

				//Header
				MGlobal::displayInfo(MString("PositionConstraint: ") +index);
				
				//Name
				MFnDependencyNode fnDepNodePosConstraint(oPositionConstraint);
				MString msg = MString(fnDepNodePosConstraint.name());
				
				//wsPos
				MString msg1 = MString("x: ") +wsPos.x;
				MString msg2 = MString(" y: ") +wsPos.y;
				MString msg3 = MString(" z: ") +wsPos.z;

				//vertexIndex
				MString msg4 = MString("VertexIndex: ") +vertexIndex;

				//Print strings
				MGlobal::displayInfo(msg);
				MGlobal::displayInfo(msg1 + msg2 + msg3);
				MGlobal::displayInfo(msg4);
				MGlobal::displayInfo("-----------------------------");

			};
		};


		//Connect posConstraintLists to solver
		//-----------------------------------------------
		for(int index = 0; index < verticesCount; index++)
		{
			//Get Plugs to clothSolverAttrs
			//-----------------------------------------------
			
			//Get plug to compound array of clothSolver
			MPlug pCompoundArrayAttr = fnDepNodeClothSolver.findPlug("positionConstraint");
			MObject aCompoundArrayAttr = pCompoundArrayAttr.attribute();
			
			//lastArrayElementIndex
			int lastArrayElementIndex = pCompoundArrayAttr.numElements();
			
			
			//plug to last array element
			MPlug pCompoundAttr = pCompoundArrayAttr.elementByLogicalIndex(lastArrayElementIndex);

			//Get Plugs to children
			MPlug pTargetConstraintActive = pCompoundAttr.child(0);
			MPlug pTargetConstraintVertexIndex = pCompoundAttr.child(1);
			
			MPlug pTargetCompoundConstraintCoordinate = pCompoundAttr.child(2);
			MPlug pTargetConstraintCoordinateX = pTargetCompoundConstraintCoordinate.child(0);
			MPlug pTargetConstraintCoordinateY = pTargetCompoundConstraintCoordinate.child(1);
			MPlug pTargetConstraintCoordinateZ = pTargetCompoundConstraintCoordinate.child(2);

			

			
			
			//Get Plug to locator attrs
			//-----------------------------------------------
			
			//get Plug to posConstraint (locatorShape) worldPos attr
			//get shape of loc from transform
			MFnDagNode fnDagNodePosConstraint(posConstraintObjectList[index]);
			MObject oPosConstraintShape = fnDagNodePosConstraint.child(0);
			
			MFnDependencyNode fnDepNodePosConstraintShape(oPosConstraintShape);
			MPlug pSourceCompoundArrayAttr = fnDepNodePosConstraintShape.findPlug("worldPosition");
			
			MGlobal::displayInfo(fnDepNodePosConstraintShape.name());

			//plug to first array element
			MPlug pCompoundPositionAttr = pSourceCompoundArrayAttr.elementByLogicalIndex(0);
			MPlug pSourceConstraintCoordinateX = pCompoundPositionAttr.child(0);
			MPlug pSourceConstraintCoordinateY = pCompoundPositionAttr.child(1);
			MPlug pSourceConstraintCoordinateZ = pCompoundPositionAttr.child(2);


			//connect position plugs
			MDGModifier dgModifier;
			dgModifier.connect(pSourceConstraintCoordinateX, pTargetConstraintCoordinateX);
			dgModifier.connect(pSourceConstraintCoordinateY, pTargetConstraintCoordinateY);
			dgModifier.connect(pSourceConstraintCoordinateZ, pTargetConstraintCoordinateZ);
			dgModifier.doIt();

			//Set vertexIndex & constraintActive
			pTargetConstraintActive.setBool(true);
			pTargetConstraintVertexIndex.setInt(posConstraintVertexIndexList[index]);

		};
		
		
		
		//delete dynamic ptr
		delete [] posConstraintPositionList;
		delete [] posConstraintVertexIndexList;
		delete [] posConstraintObjectList;

		//advance
		itSelectionList.next();
	};

	return MS::kSuccess;
};