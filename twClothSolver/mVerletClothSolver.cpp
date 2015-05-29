

//MVerletClothSolver Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "mVerletClothSolver.h"


//MVerletClothSolver
//-----------------------------------------------

//Attributes
MTypeId MVerletClothSolver::id(0x70008);
MString MVerletClothSolver::typeName("verletClothSolver");

MObject MVerletClothSolver::aVerbose;
MObject MVerletClothSolver::aVerboseStaticSolverData;
MObject MVerletClothSolver::aVerboseDynamicSolverData;
MObject MVerletClothSolver::aVerboseDrawData;

MObject MVerletClothSolver::aSolverType;

MObject MVerletClothSolver::aInputGeo;
MObject MVerletClothSolver::aOutputGeo;

MObject MVerletClothSolver::aStartFrame;
MObject MVerletClothSolver::aCurrentFrame;

MObject MVerletClothSolver::aGravity;

MObject MVerletClothSolver::aTransformMatrix;

MObject MVerletClothSolver::aDrag;

MObject MVerletClothSolver::aCollisionConstraintGroundplaneActive;
MObject MVerletClothSolver::aCollisionConstraintGroundplaneHeight;
MObject MVerletClothSolver::aCollisionConstraintGroundplaneVisible;
MObject MVerletClothSolver::aCollisionConstraintGroundplaneDispSize;

MObject MVerletClothSolver::aCollisionConstraint;
MObject MVerletClothSolver::aCollisionConstraintActive;
MObject MVerletClothSolver::aCollisionConstraintType;
MObject MVerletClothSolver::aCollisionConstraintGeoMatrix;
MObject MVerletClothSolver::aCollisionConstraintGeo;

MObject MVerletClothSolver::aCollisionConstraintConvexOffset;

MObject MVerletClothSolver::aRepetitions;

MObject MVerletClothSolver::aPositionConstraint;
MObject MVerletClothSolver::aPositionConstraintActive;
MObject MVerletClothSolver::aPositionConstraintVertexIndex;
MObject MVerletClothSolver::aPositionConstraintCoordinate;

MObject MVerletClothSolver::aDrawMeshlinesActive;
MObject MVerletClothSolver::aDrawMeshlinesColorMode;

MObject MVerletClothSolver::aSolveTriangulated;

MObject MVerletClothSolver::a_thread_count;
MObject MVerletClothSolver::a_omp;

//Methods
MVerletClothSolver::MVerletClothSolver(){}
MVerletClothSolver::~MVerletClothSolver(){}


//create
//-----------------------------------------------
void* MVerletClothSolver::create()
{
	return new MVerletClothSolver();
}


//initialize
//-----------------------------------------------
MStatus MVerletClothSolver::initialize()
{
	
	//MFnSets
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnCompoundAttribute cAttr;

	
	//aVerbose
	aVerbose = eAttr.create("verbose", "verbose", 0);
	eAttr.addField("noVerbose", 0);
	eAttr.addField("Verbose", 1);
	addAttribute(aVerbose);
	//aVerboseStaticSolverData
	aVerboseStaticSolverData = eAttr.create("verboseStaticSolverData", "verboseStaticSolverData", 0);
	eAttr.addField("noVerbose", 0);
	eAttr.addField("Verbose", 1);
	addAttribute(aVerboseStaticSolverData);
	//aVerboseDynamicSolverData
	aVerboseDynamicSolverData = eAttr.create("verboseDynamicSolverData", "verboseDynamicSolverData", 0);
	eAttr.addField("noVerbose", 0);
	eAttr.addField("Verbose", 1);
	addAttribute(aVerboseDynamicSolverData);
	//aVerboseDrawData
	aVerboseDrawData = eAttr.create("verboseDrawData", "verboseDrawData", 0);
	eAttr.addField("noVerbose", 0);
	eAttr.addField("Verbose", 1);
	addAttribute(aVerboseDrawData);

	//aSolverType
	aSolverType = eAttr.create("solverType", "solverType", 1);
	eAttr.addField("CPU", 1);
	eAttr.addField("GPU", 2);
	addAttribute(aSolverType);

	//aInputGeo
	aInputGeo = tAttr.create("inputGeo", "inputGeo", MFnData::kMesh);
	tAttr.setReadable(false);
	addAttribute(aInputGeo);
	
	//aOutputGeo
	aOutputGeo = tAttr.create("outputGeo", "outputGeo", MFnData::kMesh);
	tAttr.setWritable(false);
	tAttr.setStorable(false);
	addAttribute(aOutputGeo);

	//aStartFrame
	aStartFrame = nAttr.create("startFrame", "startFrame", MFnNumericData::kFloat, 1.0);
	addAttribute(aStartFrame);

	//aCurrentFrame
	aCurrentFrame = nAttr.create("currentFrame", "currentFrame", MFnNumericData::kFloat);
	addAttribute(aCurrentFrame);

	//aGravity
	aGravity = nAttr.createPoint("gravity", "gravity");
	nAttr.setKeyable(true);
	addAttribute(aGravity);

	//aTransformMatrix
	aTransformMatrix = mAttr.create("transformMatrix", "transformMatrix");
	mAttr.setReadable(false);
	mAttr.setHidden(true);
	addAttribute(aTransformMatrix);

	//aDrag
	aDrag = nAttr.create("drag", "drag", MFnNumericData::kFloat , 0.0);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setKeyable(true);
	addAttribute(aDrag);

	//aCollisionConstraintGroundplaneActive
	aCollisionConstraintGroundplaneActive = nAttr.create("collisionConstraintGroundplaneActive","collisionConstraintGroundplaneActive", MFnNumericData::kBoolean, 0);
	nAttr.setStorable(true);
	addAttribute(aCollisionConstraintGroundplaneActive);
	//aCollisionConstraintGroundplaneHeight
	aCollisionConstraintGroundplaneHeight = nAttr.create("collisionConstraintGroundplaneHeight", "collisionConstraintGroundplaneHeight", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(true);
	addAttribute(aCollisionConstraintGroundplaneHeight);
	//aCollisionConstraintGroundplaneVisible
	aCollisionConstraintGroundplaneVisible = nAttr.create("collisionConstraintGroundplaneVisible", "collisionConstraintGroundplaneVisible", MFnNumericData::kBoolean, 0);
	nAttr.setStorable(true);
	addAttribute(aCollisionConstraintGroundplaneVisible);
	//aCollisionConstraintGroundplaneDispSize
	aCollisionConstraintGroundplaneDispSize = nAttr.create("collisionConstraintGroundplaneDispSize", "collisionConstraintGroundplaneDispSize", MFnNumericData::kFloat, 1.0);
	nAttr.setKeyable(true);
	nAttr.setHidden(false);
	addAttribute(aCollisionConstraintGroundplaneDispSize);


	//aCollisionConstraintActive
	aCollisionConstraintActive = eAttr.create("collisionConstraintActive", "collisionConstraintActive", 0);
	eAttr.addField("Inactive", 0);
	eAttr.addField("Active", 1);
	//aCollisionConstraintType
	aCollisionConstraintType = eAttr.create("collisionConstraintType", "collisionConstraintType", 0);
	eAttr.addField("Sphere", 0);
	eAttr.addField("ConvexPrimitive", 1);
	//aCollisionConstraintGeoMatrix
	aCollisionConstraintGeoMatrix = mAttr.create("collisionConstraintGeoMatrix", "collisionConstraintGeoMatrix");
	mAttr.setReadable(false);
	//aCollisionConstraintGeo
	aCollisionConstraintGeo = tAttr.create("collisionConstraintGeo", "collisionConstraintGeo", MFnData::kMesh);
	tAttr.setWritable(true);
	tAttr.setReadable(false);

	//aCollisionConstraint
	aCollisionConstraint = cAttr.create("collisionConstraint", "collisionConstraint");
	cAttr.setArray(true);
	cAttr.setUsesArrayDataBuilder(true);
	cAttr.addChild(aCollisionConstraintActive);
	cAttr.addChild(aCollisionConstraintType);
	cAttr.addChild(aCollisionConstraintGeoMatrix);
	cAttr.addChild(aCollisionConstraintGeo);
	addAttribute(aCollisionConstraint);


	//aCollisionConstraintConvexOffset
	aCollisionConstraintConvexOffset = nAttr.create("collisionConstraintConvexOffset", "collisionConstraintConvexOffset", MFnNumericData::kFloat , 0.1);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setKeyable(true);
	addAttribute(aCollisionConstraintConvexOffset);


	//aRepetitions
	aRepetitions = nAttr.create("repetitions", "repetitions", MFnNumericData::kInt, 1);
	nAttr.setKeyable(true);
	addAttribute(aRepetitions);

	//aPositionConstraintActive
	aPositionConstraintActive = eAttr.create("positionConstraintActive", "positionConstraintActive");
	eAttr.addField("Inactive", 0);
	eAttr.addField("Active", 1);
	eAttr.setDefault(0);
	//aPositionConstraintVertexIndex
	aPositionConstraintVertexIndex = nAttr.create("positionConstraintVertexIndex", "positionConstraintVertexIndex", MFnNumericData::kInt, 0);
	nAttr.setKeyable(true);
	//aPositionConstraintCoordinate
	aPositionConstraintCoordinate = nAttr.createPoint("positionConstraintCoordinate", "positionConstraintCoordinate");
	nAttr.setKeyable(true);

	//aPositionConstraint
	aPositionConstraint = cAttr.create("positionConstraint", "positionConstraint");
	cAttr.setArray(true);
	cAttr.setUsesArrayDataBuilder(true);
	cAttr.addChild(aPositionConstraintActive);
	cAttr.addChild(aPositionConstraintVertexIndex);
	cAttr.addChild(aPositionConstraintCoordinate);
	addAttribute(aPositionConstraint);


	//aDrawMeshlinesActive
	aDrawMeshlinesActive = nAttr.create("drawMeshlinesActive", "drawMeshlinesActive", MFnNumericData::kBoolean, 0);
	nAttr.setStorable(true);
	addAttribute(aDrawMeshlinesActive);
	//aDrawMeshlinesColorMode
	aDrawMeshlinesColorMode = eAttr.create("drawMeshlinesColorMode", "drawMeshlinesColorMode", 0);
	eAttr.addField("Mono", 0);
	eAttr.addField("Distortion", 1);
	eAttr.addField("Change", 2);
	addAttribute(aDrawMeshlinesColorMode);

	//aSolveTriangulated
	aSolveTriangulated = nAttr.create("solveTriangulated", "solveTriangulated", MFnNumericData::kBoolean, 0);
	nAttr.setStorable(true);
	addAttribute(aSolveTriangulated);

	//a_thread_count
	a_thread_count = nAttr.create("thread_count", "thread_count", MFnNumericData::kInt, 256);
	nAttr.setMin(1);
	nAttr.setMax(512);
	//nAttr.setKeyable(true);
	addAttribute(a_thread_count);

	//a_omp
	a_omp = eAttr.create("omp", "omp", 0);
	eAttr.addField("OMP Support Off", 0);
	eAttr.addField("OMP Support On", 1);
	addAttribute(a_omp);

	//AttributeAffects
	//Simulation
	attributeAffects(aCurrentFrame, aOutputGeo);
	//Drawing
	attributeAffects(aCollisionConstraintGroundplaneHeight, aOutputGeo);
	attributeAffects(aCollisionConstraintGroundplaneVisible, aOutputGeo);
	attributeAffects(aCollisionConstraintGroundplaneDispSize, aOutputGeo);
	attributeAffects(aDrawMeshlinesActive, aOutputGeo);
	attributeAffects(aDrawMeshlinesColorMode, aOutputGeo);

	return MStatus::kSuccess;
}


//compute
//-----------------------------------------------
MStatus MVerletClothSolver::compute(const MPlug &plug, MDataBlock &data)
{
	
	//get attributes from datablock
	//-----------------------------------------------
	short verbose = data.inputValue(aVerbose).asShort();
	short verboseStaticSolverData = data.inputValue(aVerboseStaticSolverData).asShort();
	short verboseDynamicSolverData = data.inputValue(aVerboseDynamicSolverData).asShort();
	short verboseDrawData = data.inputValue(aVerboseDrawData).asShort();

	int solverType = data.inputValue(aSolverType).asInt();
	float currentFrame = data.inputValue(aCurrentFrame).asFloat();
	float startFrame = data.inputValue(aStartFrame).asFloat();
	
	
	//wrong plug requested
	//-----------------------------------------------
	if(plug != aOutputGeo)
	{
		if(verbose){MGlobal::displayInfo("Plug not aOutputGeo");};
		return MStatus::kSuccess;
	}
	//right plug requested
	//-----------------------------------------------
	else
	{
		//Check if all attrs connected
		//-----------------------------------------------
		if(!attributeConnected(MString("inputGeo")))
		{
			if(verbose){MGlobal::displayInfo("Attribute inputGeo not connected");};
		}

		else if(!attributeConnected(MString("transformMatrix")))
		{
			if(verbose){MGlobal::displayInfo("Attribute transformMatrix not connected");};
		}


		//Check if currentFrame <= startframe
		//-----------------------------------------------
		else if(currentFrame <= startFrame)
		{
			//set static solver data
			setStaticSolverData(&data);
			if(VerletClothSolverGPU::cudaAvailable() == 1)
				set_static_solver_data_gpu();
			if(verboseStaticSolverData)
				printStaticSolverData();

			//set dynamic solver data
			setDynamicSolverData(&data);
			if(VerletClothSolverGPU::cudaAvailable() == 1 && solverType == 2)
				set_dynamic_solver_data_gpu();
			if(verboseDynamicSolverData)
				printDynamicSolverData();

			//set draw data
			setDrawData(data);
			if(verboseDrawData)
				printDrawData();

			//set lastFrame
			lastFrame = currentFrame;

			//Set output geo
			setOutputGeo(data);
			//set plug clean
			data.setClean(plug);

			//Temp
			
		}

		//solve
		//-----------------------------------------------
		else
		{	
			//set static solver data if not set
			if(!is_static_solver_data_initialized())
			{
				setStaticSolverData(&data);
				if(VerletClothSolverGPU::cudaAvailable() == 1)
					set_static_solver_data_gpu();
				if(verboseStaticSolverData)
					printStaticSolverData();
			};
			
			//set draw data
			setDrawData(data);
			if(verboseDrawData)
				printDrawData();

			//If lastFrame < CurrentFrame then solve
			if(lastFrame < currentFrame)
			{
				//set dynamic solver data
				setDynamicSolverData(&data);
				if(VerletClothSolverGPU::cudaAvailable() == 1 && solverType == 2)
					set_dynamic_solver_data_gpu();
				if(verboseDynamicSolverData)
					printDynamicSolverData();
				
				//solve
				solve(solver_t(solverType));
				//set solverType in interface
				data.outputValue(aSolverType).set(int(getType()));
			}

			//set lastFrame
			lastFrame = currentFrame;

			//Set output geo
			if(VerletClothSolverGPU::cudaAvailable() == 1 && solverType == 2)
				set_static_solver_data_from_gpu();
			setOutputGeo(data);
			//set plug clean
			data.setClean(plug);

			//Temp
			
		}

		
		return MStatus::kSuccess;
	}
}


//draw
//-----------------------------------------------
void MVerletClothSolver::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
	//CollisionGroundPlane
	drawCollisionGroundplane(view, path, style, displayStatus);
	//drawMeshlines
	drawMeshlines(view, path, style, displayStatus);

	//MGlobal::displayInfo("MVerletClothSolver::draw");
}


//drawCollisionGroundplane
//-----------------------------------------------
void MVerletClothSolver::drawCollisionGroundplane(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus)
{

	//get drawData
	int displayCollisionGroundplane = drawData.collisionConstraintGroundPlaneVisible;
	float collisionGroundplaneDisplaySize = drawData.collisionConstraintGroundplaneDispSize;
	float collisionConstraintGroundplaneHeight = drawData.collisionConstraintGroundplaneHeight;

	if(displayCollisionGroundplane)
	{
		//Get glFT
		static MGLFunctionTable* pGlFT = 0;
		if ( 0 == pGlFT ){pGlFT = MHardwareRenderer::theRenderer()->glFunctionTable();};

		// Begin the drawing
		view.beginGL();

		//Store current settings
		pGlFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);

		//Make transparent
		pGlFT->glEnable(GL_BLEND);
		pGlFT->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		//Draw Quad
		pGlFT->glBegin(MGL_QUADS);
			//Color
			pGlFT->glColor4f(1.0f, 0.0f,0.0f, 0.3f);
			//QuadVertices
			pGlFT->glVertex3f( -1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( 1.0* collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( 1.0* collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( -1.0* collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);
		pGlFT->glEnd();

		//Reset transparency
		pGlFT->glDisable(GL_BLEND);

		
		//Draw Frame
		pGlFT->glBegin(MGL_LINES);
			//Color
			pGlFT->glColor3f(1.0f, 1.0f,0.0f);
			//Lines
			pGlFT->glVertex3f( -1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( 1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);

			pGlFT->glVertex3f( 1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( 1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);
			
			pGlFT->glVertex3f( 1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( -1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);

			pGlFT->glVertex3f( -1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, 1.0 * collisionGroundplaneDisplaySize);
			pGlFT->glVertex3f( -1.0 * collisionGroundplaneDisplaySize, collisionConstraintGroundplaneHeight, -1.0 * collisionGroundplaneDisplaySize);

		pGlFT->glEnd();

		
		//Reset current settings
		pGlFT->glPopAttrib();

		view.endGL();

	}
	else
	{
		//std::cout<<"Drawing of collision groundplane inactive"<<std::endl;
	};

};

//drawMeshlines
//-----------------------------------------------
void MVerletClothSolver::drawMeshlines(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{

	//Get drawData
	int drawMeshlines = drawData.drawMeshlinesActive;

	if(drawMeshlines)
	{	
		//Get glFT
		static MGLFunctionTable* pGlFT = 0;
		if ( 0 == pGlFT ){pGlFT = MHardwareRenderer::theRenderer()->glFunctionTable();};

		// Begin the drawing
		view.beginGL();

		//Store current settings
		pGlFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);

		//LineWidth
		pGlFT->glLineWidth(5.0);
		
		//Draw Frame
		pGlFT->glBegin(MGL_LINES);

		//Iterate edgeVertexIndexList
		for(int index = 0; index < staticSolverData.edgeCount; index++)
		{
			//Indices
			int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
			int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);

			//Color
			//get color for edgeIndex
			vcsVector col1 = drawData.pVertexColorList[vertexIndex1];
			vcsVector col2 = drawData.pVertexColorList[vertexIndex2];

			//Position
			//get vcsVector for edgeIndex
			vcsVector vec1 = staticSolverData.pVertexPositionList[vertexIndex1];
			vcsVector vec2 = staticSolverData.pVertexPositionList[vertexIndex2];

			//ogl vertex1
			pGlFT->glColor3f(col1.x, col1.y, col1.z);
			pGlFT->glVertex3f(vec1.x, vec1.y, vec1.z);
			//ogl vertex2
			pGlFT->glColor3f(col2.x, col2.y, col2.z);
			pGlFT->glVertex3f(vec2.x, vec2.y, vec2.z);
		};

		//end
		pGlFT->glEnd();

		//Reset current settings
		pGlFT->glPopAttrib();

		view.endGL();
	};

};



//staticSolverData
//-----------------------------------------------

//setStaticSolverData
//-----------------------------------------------
void MVerletClothSolver::setStaticSolverData(void* proprietaryData)
{
	//cast MDataBlockPtr
	MDataBlock* pData = (MDataBlock*)proprietaryData;

	//Execute setting of member data
	setVertexCount(*pData);
	setVertexPositionList(*pData);
	setVertexOldpositionList(*pData);
	setVertexInitialpositionList(*pData);
	setEdgeVertexIndexListAndCount(*pData);
	setEdgeRestlengthList(*pData);
	
	//MGlobal::displayInfo("Set static solver data");
};

//printStaticSolverData
//-----------------------------------------------
void MVerletClothSolver::printStaticSolverData()
{
	MGlobal::displayInfo("Static Solver Data");
	MGlobal::displayInfo("----------------------------------------------");

	//vertexCount
	MGlobal::displayInfo(MString("vertexCount: ") +staticSolverData.vertexCount);

	//vertexPositionList
	MGlobal::displayInfo("vertexPositionList:");
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float x = staticSolverData.pVertexPositionList[index].x;
		float y = staticSolverData.pVertexPositionList[index].y;
		float z = staticSolverData.pVertexPositionList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MString msgY = MString(" Y: ") +y;
		MString msgZ = MString(" Z: ") +z;
		MGlobal::displayInfo(msgIndex +msgX +msgY + msgZ);
	};

	//vertexOldpositionList
	MGlobal::displayInfo("vertexOldpositionList:");
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float x = staticSolverData.pVertexOldpositionList[index].x;
		float y = staticSolverData.pVertexOldpositionList[index].y;
		float z = staticSolverData.pVertexOldpositionList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MString msgY = MString(" Y: ") +y;
		MString msgZ = MString(" Z: ") +z;
		MGlobal::displayInfo(msgIndex +msgX +msgY + msgZ);
	};

	//vertexInitialpositionList
	MGlobal::displayInfo("vertexInitialpositionList:");
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float x = staticSolverData.pVertexInitialpositionList[index].x;
		float y = staticSolverData.pVertexInitialpositionList[index].y;
		float z = staticSolverData.pVertexInitialpositionList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MString msgY = MString(" Y: ") +y;
		MString msgZ = MString(" Z: ") +z;
		MGlobal::displayInfo(msgIndex +msgX +msgY + msgZ);
	};

	//edgeCount
	MGlobal::displayInfo(MString("edgeCount: ") +staticSolverData.edgeCount);
	
	//edgeVertexIndexList
	MGlobal::displayInfo("edgeVertexIndexList:");
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		int x = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int y = int(staticSolverData.pEdgeVertexIndexList[index].y);
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MString msgY = MString(" Y: ") +y;
		MGlobal::displayInfo(msgIndex +msgX +msgY);
	};

	//edgeRestlengthList
	MGlobal::displayInfo("edgeRestlengthList:");
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		float x = staticSolverData.pEdgeRestlengthList[index];
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MGlobal::displayInfo(msgIndex +msgX);
	};

};

//setVertexCount
//-----------------------------------------------
void MVerletClothSolver::setVertexCount(MDataBlock &data)
{
	//Get inputGeo from datablock
	MObject inputGeo = data.inputValue(aInputGeo).asMesh();
	//FnSet
	MFnMesh fsInputGeo(inputGeo);
	//numVertices
	staticSolverData.vertexCount = fsInputGeo.numVertices();
};

//setVertexPositionList
//-----------------------------------------------
void MVerletClothSolver::setVertexPositionList(MDataBlock &data)
{
	//Get inputGeo from datablock
	MObject inputGeo = data.inputValue(aInputGeo).asMesh();
	//FnSet
	MFnMesh fsInputGeo(inputGeo);
	//pointArray
	MPointArray inputGeoPointArray;
	fsInputGeo.getPoints(inputGeoPointArray);

	//get transformMatrix from dataBlock
	MMatrix transformMatrix = data.inputValue(aTransformMatrix).asMatrix();
	
	//vcsVector array
	delete [] staticSolverData.pVertexPositionList;
	staticSolverData.pVertexPositionList = new vcsVector [staticSolverData.vertexCount];

	//fill vcsVectorArray with points in ws
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{	
		MPoint point = inputGeoPointArray[index]*transformMatrix;
		float x = point.x;
		float y = point.y;
		float z = point.z;
		staticSolverData.pVertexPositionList[index] = vcsVector(x, y, z);
	};

};

//setVertexOldpositionList
//-----------------------------------------------
void MVerletClothSolver::setVertexOldpositionList(MDataBlock &data)
{
	//Get inputGeo from datablock
	MObject inputGeo = data.inputValue(aInputGeo).asMesh();
	//FnSet
	MFnMesh fsInputGeo(inputGeo);
	//pointArray
	MPointArray inputGeoPointArray;
	fsInputGeo.getPoints(inputGeoPointArray);

	//get transformMatrix from dataBlock
	MMatrix transformMatrix = data.inputValue(aTransformMatrix).asMatrix();
	
	//vcsVector array
	delete [] staticSolverData.pVertexOldpositionList;
	staticSolverData.pVertexOldpositionList = new vcsVector [staticSolverData.vertexCount];

	//fill vcsVectorArray with points in ws
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{	
		MPoint point = inputGeoPointArray[index]*transformMatrix;
		float x = point.x;
		float y = point.y;
		float z = point.z;
		staticSolverData.pVertexOldpositionList[index] = vcsVector(x, y, z);
	};

};

//setVertexInitialpositionList
//-----------------------------------------------
void MVerletClothSolver::setVertexInitialpositionList(MDataBlock &data)
{
	//Get inputGeo from datablock
	MObject inputGeo = data.inputValue(aInputGeo).asMesh();
	//FnSet
	MFnMesh fsInputGeo(inputGeo);
	//pointArray
	MPointArray inputGeoPointArray;
	fsInputGeo.getPoints(inputGeoPointArray);

	//get transformMatrix from dataBlock
	MMatrix transformMatrix = data.inputValue(aTransformMatrix).asMatrix();
	
	//vcsVector array
	delete [] staticSolverData.pVertexInitialpositionList;
	staticSolverData.pVertexInitialpositionList = new vcsVector [staticSolverData.vertexCount];

	//fill vcsVectorArray with points in ws
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{	
		MPoint point = inputGeoPointArray[index]*transformMatrix;
		float x = point.x;
		float y = point.y;
		float z = point.z;
		staticSolverData.pVertexInitialpositionList[index] = vcsVector(x, y, z);
	};

};


//setEdgeVertexIndexListAndCount
//-----------------------------------------------
void MVerletClothSolver::setEdgeVertexIndexListAndCount(MDataBlock &data)
{
	//Get inputGeo from datablock
	MObject oInputGeo = data.inputValue(aInputGeo).asMesh();
	//FnSet
	MFnMesh fsInputGeo(oInputGeo);
	
	//get solveTriangulated
	bool solveTriangulated = data.inputValue(aSolveTriangulated).asBool();

	//get edge iterator
	MItMeshEdge itMeshEdges(oInputGeo);
	
	//delete content of old pointer
	delete [] staticSolverData.pEdgeVertexIndexList;
	
	
	
	//Solve NOT triangulated
	//-----------------------------------------------
	if(!solveTriangulated)
	{
		//Allocate memory for new ptr
		staticSolverData.pEdgeVertexIndexList = new vcsVector[itMeshEdges.count()];
	
		//iterate edges
		while(!itMeshEdges.isDone())
		{
			//get vertexIds for current edge
			int2 vertexIds;
			fsInputGeo.getEdgeVertices(itMeshEdges.index(), vertexIds);
			//store in edgeVertexIndexList
			staticSolverData.pEdgeVertexIndexList[itMeshEdges.index()] = vcsVector(float(vertexIds[0]), float(vertexIds[1]), 0.0);
			//next
			itMeshEdges.next();
		};
		//set edgeCount
		staticSolverData.edgeCount = itMeshEdges.count();
		
	}
	
	
	//Solve triangulated
	//-----------------------------------------------
	else
	{
		/*
		ORIGINAL
		//Get triangleVertexIndexList
		MIntArray triangleCountList;
		MIntArray triangleVertexIndexList;
		fsInputGeo.getTriangles(triangleCountList, triangleVertexIndexList);

	
		//Allocate memory for new ptr
		staticSolverData.pEdgeVertexIndexList = new vcsVector[triangleVertexIndexList.length()];
		//iterate and fill
		for(int index = 0; index < triangleVertexIndexList.length(); index = index + 3)
		{
			//get vertexIndices for triangle
			int vertexId1 = triangleVertexIndexList[index];
			int vertexId2 = triangleVertexIndexList[index+1];
			int vertexId3 = triangleVertexIndexList[index+2];

			//add triangulatedEdgeVertexIndexList
			staticSolverData.pEdgeVertexIndexList[index] = vcsVector(vertexId1, vertexId2, 0.0);
			staticSolverData.pEdgeVertexIndexList[index+1] = vcsVector(vertexId2, vertexId3, 0.0);
			staticSolverData.pEdgeVertexIndexList[index+2] = vcsVector(vertexId3, vertexId1, 0.0);
		};
	
		
		//set staticSolverData.edgeCount
		staticSolverData.edgeCount = triangleVertexIndexList.length();
		*/


		//NEW
		
		//edgeVertexPositionList;
		std::vector<std::vector<float>> edgeVertexIndexList;

		//vertexPositionList
		MPointArray vertexPositionList;
		fsInputGeo.getPoints(vertexPositionList);

		//itMeshPolyInputGeo
		MItMeshPolygon itMeshPolyInputGeo(oInputGeo);

		//for each poly
		while(!itMeshPolyInputGeo.isDone())
		{
			//get edges
			MIntArray edgeIndexList;
			itMeshPolyInputGeo.getEdges(edgeIndexList);
		
			//edgeVertexIndices
			int2* edgeVertexIndices = new int2[4];

			for(int index = 0; index < edgeIndexList.length(); index++)
			{
				fsInputGeo.getEdgeVertices(edgeIndexList[index], edgeVertexIndices[index]);
			};

			//edgeVertexIndicesNoDuplicates
			std::set<int> setEdgeVertexIndicesNoDuplicates;

			for(int index = 0; index < edgeIndexList.length(); index++)
			{
				setEdgeVertexIndicesNoDuplicates.insert(edgeVertexIndices[index][0]);
				setEdgeVertexIndicesNoDuplicates.insert(edgeVertexIndices[index][1]);
			};

			//vecEdgeVertexIndicesNoDuplicates
			std::vector<int> vecEdgeVertexIndicesNoDuplicates(setEdgeVertexIndicesNoDuplicates.begin(), setEdgeVertexIndicesNoDuplicates.end());

		
		
			//edge01
			std::vector<float> edge01;
			edge01.push_back(float(vecEdgeVertexIndicesNoDuplicates[0]));
			edge01.push_back(float(vecEdgeVertexIndicesNoDuplicates[1]));

			//edge13
			std::vector<float> edge13;
			edge13.push_back(float(vecEdgeVertexIndicesNoDuplicates[1]));
			edge13.push_back(float(vecEdgeVertexIndicesNoDuplicates[3]));

			//edge32
			std::vector<float> edge32;
			edge32.push_back(float(vecEdgeVertexIndicesNoDuplicates[3]));
			edge32.push_back(float(vecEdgeVertexIndicesNoDuplicates[2]));

			//edge20
			std::vector<float> edge20;
			edge20.push_back(float(vecEdgeVertexIndicesNoDuplicates[2]));
			edge20.push_back(float(vecEdgeVertexIndicesNoDuplicates[0]));

			//edge03
			std::vector<float> edge03;
			edge03.push_back(float(vecEdgeVertexIndicesNoDuplicates[0]));
			edge03.push_back(float(vecEdgeVertexIndicesNoDuplicates[3]));

			//edge12
			std::vector<float> edge12;
			edge12.push_back(float(vecEdgeVertexIndicesNoDuplicates[1]));
			edge12.push_back(float(vecEdgeVertexIndicesNoDuplicates[2]));
			

			//append to edgeVertexIndexList
			edgeVertexIndexList.push_back(edge01);
			edgeVertexIndexList.push_back(edge13);
			edgeVertexIndexList.push_back(edge32);
			edgeVertexIndexList.push_back(edge20);
			edgeVertexIndexList.push_back(edge03);
			edgeVertexIndexList.push_back(edge12);


			//del edgeVertexIndices
			delete [] edgeVertexIndices;
		
			//Advance
			itMeshPolyInputGeo.next();
		};

		//Allocate memory for new ptr
		int shearEdgeCount = edgeVertexIndexList.size();
		staticSolverData.pEdgeVertexIndexList = new vcsVector[shearEdgeCount];

		//iterate and set
		for(int index = 0; index < shearEdgeCount; index++)
		{
			//indices
			float index1 = edgeVertexIndexList[index][0];
			float index2 = edgeVertexIndexList[index][1];

			//set to staticSolverData.pEdgeVertexIndexList
			staticSolverData.pEdgeVertexIndexList[index] = vcsVector(index1, index2, 0.0);
		};

		//set staticSolverData.edgeCount
		staticSolverData.edgeCount = shearEdgeCount;


	};
};

//setEdgeRestlengthList
//-----------------------------------------------
void MVerletClothSolver::setEdgeRestlengthList(MDataBlock &data)
{
	//delete old ptr content and allocate new memory
	delete [] staticSolverData.pEdgeRestlengthList;
	staticSolverData.pEdgeRestlengthList = new float [staticSolverData.edgeCount];
	
	//iterate edgeVertexIndexList
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//Get vertexIndices for edge
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);
		//get length from vcsVectors at indices
		vcsVector vec1 = staticSolverData.pVertexPositionList[vertexIndex1];
		vcsVector vec2 = staticSolverData.pVertexPositionList[vertexIndex2];
		//compute restlength
		float restLength = (vec1 - vec2).length();
		//set in edgeRestlengthList
		staticSolverData.pEdgeRestlengthList[index] = restLength;
	};

};



//dynamicSolverData
//-----------------------------------------------

//setDynamicSolverData
//-----------------------------------------------
void MVerletClothSolver::setDynamicSolverData(void* proprietaryData)
{
	//cast MDataBlockPtr
	MDataBlock* pData = static_cast<MDataBlock*>(proprietaryData);

	dynamicSolverData.drag = pData->inputValue(aDrag).asFloat();
	dynamicSolverData.collisionGroundplaneActive = pData->inputValue(aCollisionConstraintGroundplaneActive).asInt();
	dynamicSolverData.groundplaneHeight = pData->inputValue(aCollisionConstraintGroundplaneHeight).asFloat();
	setVertexForceList(*pData);
	dynamicSolverData.repetitions = pData->inputValue(aRepetitions).asInt();
	setPositionConstraintCount(*pData);
	setPositionConstraintActiveList(*pData);
	setPositionConstraintVertexIndexList(*pData);
	setPositionConstraintCoordinateList(*pData);
	setCollisionConstraintSpheresCountAndVecUpVecDownList(*pData);
	setCollisionConstraintConvexCountAndTriangleCountList(*pData);
	setCollisionConstraintConvexTriangleVertexPositionList(*pData);
	setCollisionConstraintConvexOffset(*pData);

	set_thread_count_and_omp(*pData);

	//MGlobal::displayInfo("Set dynamic solver data");
};

//printDynamicSolverData
//-----------------------------------------------
void MVerletClothSolver::printDynamicSolverData()
{
	MGlobal::displayInfo("Dynamic Solver Data");
	MGlobal::displayInfo("----------------------------------------------");

	//drag
	MGlobal::displayInfo(MString("drag: ") +dynamicSolverData.drag);

	//vertexForceList
	MGlobal::displayInfo("vertexForceList:");
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float x = dynamicSolverData.pVertexForceList[index].x;
		float y = dynamicSolverData.pVertexForceList[index].y;
		float z = dynamicSolverData.pVertexForceList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" X: ") +x;
		MString msgY = MString(" Y: ") +y;
		MString msgZ = MString(" Z: ") +z;
		MGlobal::displayInfo(msgIndex +msgX +msgY + msgZ);
	};

	//repetitions
	MGlobal::displayInfo(MString("repetitions: ") +dynamicSolverData.repetitions);

	//positionConstraintCount
	MGlobal::displayInfo(MString("positionConstraintCount: ") +dynamicSolverData.positionConstraintCount);

	//positionConstraintActiveList
	MGlobal::displayInfo("positionConstraintActiveList:");
	for(int index = 0; index < dynamicSolverData.positionConstraintCount; index++)
	{
		int x = dynamicSolverData.pPositionConstraintActiveList[index];
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" ActiveStatus: ") +x;
		MGlobal::displayInfo(msgIndex +msgX);
	};

	//positionConstraintVertexIndexList
	MGlobal::displayInfo("positionConstraintVertexIndexList:");
	for(int index = 0; index < dynamicSolverData.positionConstraintCount; index++)
	{
		int x = dynamicSolverData.pPositionConstraintVertexIndexList[index];
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" VertexId: ") +x;
		MGlobal::displayInfo(msgIndex +msgX);
	};

	//positionConstraintCoordinateList
	MGlobal::displayInfo("positionConstraintCoordinateList:");
	for(int index = 0; index < dynamicSolverData.positionConstraintCount; index++)
	{
		float x = dynamicSolverData.pPositionConstraintCoordinateList[index].x;
		float y = dynamicSolverData.pPositionConstraintCoordinateList[index].y;
		float z = dynamicSolverData.pPositionConstraintCoordinateList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgX = MString(" posCon X: ") +x;
		MString msgY = MString(" posCon Y: ") +y;
		MString msgZ = MString(" posCon Z: ") +z;
		MGlobal::displayInfo(msgIndex +msgX +msgY + msgZ);
	};

	//collisionConstraintSpheresCount
	MGlobal::displayInfo(MString("collisionConstraintSpheresCount: ") +dynamicSolverData.collisionConstraintSpheresCount);

	//CollisionConstraintSpheresVecUpVecDownList
	MGlobal::displayInfo("CollisionConstraintSpheresVecUpVecDownList:");
	for(int index = 0; index < dynamicSolverData.collisionConstraintSpheresCount * 2; index+=2)
	{	
		//index
		MGlobal::displayInfo(MString("CollisionConstraintSphereIndex: ") +index/2);
		
		//VecUp
		float vecUpX = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index].x;
		float vecUpY = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index].y;
		float vecUpZ = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index].z;
		MString msgVecUp = MString("VecUp: ");
		MString msgVecUpX = MString(" X: ") +vecUpX;
		MString msgVecUpY = MString(" Y: ") +vecUpY;
		MString msgVecUpZ = MString(" Z: ") +vecUpZ;
		MGlobal::displayInfo(msgVecUp +msgVecUpX +msgVecUpY + msgVecUpZ);

		//VecDown
		float vecDownX = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index+1].x;
		float vecDownY = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index+1].y;
		float vecDownZ = dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[index+1].z;
		MString msgVecDown = MString("VecDown: ");
		MString msgVecDownX = MString(" X: ") +vecDownX;
		MString msgVecDownY = MString(" Y: ") +vecDownY;
		MString msgVecDownZ = MString(" Z: ") +vecDownZ;
		MGlobal::displayInfo(msgVecDown +msgVecDownX +msgVecDownY + msgVecDownZ);
	};

	//collisionConstraintConvexCount
	MGlobal::displayInfo(MString("collisionConstraintConvexCount: ") +dynamicSolverData.collisionConstraintConvexCount);

	//collisionConstraintConvexTriangleCountList
	MGlobal::displayInfo("collisionConstraintConvexTriangleCountList:");
	for(int index = 0; index < dynamicSolverData.collisionConstraintConvexCount; index++)
	{
		int triangleCount = dynamicSolverData.pCollisionConstraintConvexTriangleCountList[index];
		MString msgIndex = MString("Index: ") +index;
		MString msgTriangleCount = MString(" TriangleCount: ") +triangleCount;
		MGlobal::displayInfo(msgIndex +msgTriangleCount);
	};

	//collisionConstraintConvexTriangleVertexPositionList
	MGlobal::displayInfo("collisionConstraintConvexTriangleVertexPositionList:");
	//iterate collisionConvexCount
	for(int index = 0; index < dynamicSolverData.collisionConstraintConvexCount; index++)
	{
		//get triangleStartCount
		int triangleStartCount = 0;
		for(int triangleStartCountIndex = 0; triangleStartCountIndex < index; triangleStartCountIndex++)
		{
			triangleStartCount += dynamicSolverData.pCollisionConstraintConvexTriangleCountList[triangleStartCountIndex];
		};

		//triangleEndCount
		int triangleEndCount = triangleStartCount + dynamicSolverData.pCollisionConstraintConvexTriangleCountList[index];

		//triangleVertexStartCount & EndCount
		int triangleVertexStartCount = triangleStartCount * 3;
		int triangleVertexEndCount = triangleEndCount * 3;

		//Print
		MString msgIndex = MString("Collision Constraint Convex Index: ") +index;
		MGlobal::displayInfo(msgIndex);
		int runtimeVar = 0;
		for(int triangleVertexIndex = triangleVertexStartCount; triangleVertexIndex < triangleVertexEndCount; triangleVertexIndex++)
		{
			//Vec
			float vecX = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex].x;
			float vecY = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex].y;
			float vecZ = dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[triangleVertexIndex].z;
			MString msgVec = MString("Vec: ") +runtimeVar;
			MString msgVecX = MString(" X: ") +vecX;
			MString msgVecY = MString(" Y: ") +vecY;
			MString msgVecZ = MString(" Z: ") +vecZ;
			MGlobal::displayInfo(msgVec +msgVecX +msgVecY + msgVecZ);

			runtimeVar++;
		};
	};

	//collisionConstraintConvexOffset
	MGlobal::displayInfo(MString("collisionConstraintConvexOffset: ") +dynamicSolverData.collisionConstraintConvexOffset);

	//thread_count
	MGlobal::displayInfo(MString("GPU thread_count: ") +dynamicSolverData.thread_count);
	//omp
	MGlobal::displayInfo(MString("CPU paralellism with OMP: ") +dynamicSolverData.omp);


};

//setVertexForceList
//-----------------------------------------------
void MVerletClothSolver::setVertexForceList(MDataBlock &data)
{
	//uiFps
	float uiFps = getUiFps();

	//get gravity from datablock
	MFloatVector gravity = data.inputValue(aGravity).asFloatVector();
	
	//vcsVector array
	dynamicSolverData.pVertexForceList = new vcsVector [staticSolverData.vertexCount];

	//fill vcsVectorArray with points in ws
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{	
		float x = gravity.x;
		float y = gravity.y;
		float z = gravity.z;
		dynamicSolverData.pVertexForceList[index] = vcsVector(x, y, z) / uiFps;
	};

};

//setPositionConstraintCount
//-----------------------------------------------
void MVerletClothSolver::setPositionConstraintCount(MDataBlock &data)
{
	//get handle to positionConstraint Array attr
	MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);
	//setData
	dynamicSolverData.positionConstraintCount = hPositionConstraintArray.elementCount();

};

//setPositionConstraintActiveList
//-----------------------------------------------
void MVerletClothSolver::setPositionConstraintActiveList(MDataBlock &data)
{
	if(!dynamicSolverData.positionConstraintCount)//No posCons
	{
		//if there was posCons before delete them
		if(dynamicSolverData.pPositionConstraintActiveList)
		{
			delete [] dynamicSolverData.pPositionConstraintActiveList;
			dynamicSolverData.pPositionConstraintActiveList = 0;
		}
	}
	else//posCons 
	{
		if(dynamicSolverData.pPositionConstraintActiveList)//if there was posCons before delete them
		{
			delete [] dynamicSolverData.pPositionConstraintActiveList;
			
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);

			//allocate new memory for pPosConActiveList
			dynamicSolverData.pPositionConstraintActiveList = new int [hPositionConstraintArray.elementCount()];
			
			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintActive = hPositionConstraintArrayElement.child(aPositionConstraintActive);
				dynamicSolverData.pPositionConstraintActiveList[index] = int(hPositionConstraintActive.asShort());
			}

		}
		else//there were no posCons before 
		{
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);
			
			//allocate new memory for pPosConActiveList
			dynamicSolverData.pPositionConstraintActiveList = new int [hPositionConstraintArray.elementCount()];

			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintActive = hPositionConstraintArrayElement.child(aPositionConstraintActive);
				dynamicSolverData.pPositionConstraintActiveList[index] = int(hPositionConstraintActive.asShort());
			}
		}
	}
};

//setPositionConstraintVertexIndexList
//-----------------------------------------------
void MVerletClothSolver::setPositionConstraintVertexIndexList(MDataBlock &data)
{
	if(!dynamicSolverData.positionConstraintCount)//No posCons
	{
		//if there was posCons before delete them
		if(dynamicSolverData.pPositionConstraintVertexIndexList)
		{
			delete [] dynamicSolverData.pPositionConstraintVertexIndexList;
			dynamicSolverData.pPositionConstraintVertexIndexList = 0;
		}
	}
	else//posCons 
	{
		if(dynamicSolverData.pPositionConstraintVertexIndexList)//if there was posCons before delete them
		{
			delete [] dynamicSolverData.pPositionConstraintVertexIndexList;
			
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);

			//allocate new memory for pPosConVertexIndexList
			dynamicSolverData.pPositionConstraintVertexIndexList = new int [hPositionConstraintArray.elementCount()];
			
			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintVertexIndex = hPositionConstraintArrayElement.child(aPositionConstraintVertexIndex);
				dynamicSolverData.pPositionConstraintVertexIndexList[index] = hPositionConstraintVertexIndex.asInt();
			}

		}
		else//there were no posCons before 
		{
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);

			//allocate new memory for pPosConVertexIndexList
			dynamicSolverData.pPositionConstraintVertexIndexList = new int [hPositionConstraintArray.elementCount()];
			
			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintVertexIndex = hPositionConstraintArrayElement.child(aPositionConstraintVertexIndex);
				dynamicSolverData.pPositionConstraintVertexIndexList[index] = hPositionConstraintVertexIndex.asInt();
			}
		}
	}
};

//setPositionConstraintCoordinateList
//-----------------------------------------------
void MVerletClothSolver::setPositionConstraintCoordinateList(MDataBlock &data)
{
	if(!dynamicSolverData.positionConstraintCount)//No posCons
	{
		//if there was posCons before delete them
		if(dynamicSolverData.pPositionConstraintCoordinateList)
		{
			delete [] dynamicSolverData.pPositionConstraintCoordinateList;
			dynamicSolverData.pPositionConstraintCoordinateList = 0;
		}
	}
	else//posCons 
	{
		if(dynamicSolverData.pPositionConstraintCoordinateList)//if there was posCons before delete them
		{
			delete [] dynamicSolverData.pPositionConstraintCoordinateList;
			
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);

			//allocate new memory for pPosConVertexIndexList
			dynamicSolverData.pPositionConstraintCoordinateList = new vcsVector [hPositionConstraintArray.elementCount()];
			
			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintCoordinate = hPositionConstraintArrayElement.child(aPositionConstraintCoordinate);
				//Create new vcsVector
				float x = hPositionConstraintCoordinate.asFloatVector().x;
				float y = hPositionConstraintCoordinate.asFloatVector().y;
				float z = hPositionConstraintCoordinate.asFloatVector().z;
				dynamicSolverData.pPositionConstraintCoordinateList[index] = vcsVector(x,y,z);
			}

		}
		else//there were no posCons before 
		{
			//For Loop to assign new posConsActive
			//get handle to positionConstraint Array attr
			MArrayDataHandle hPositionConstraintArray = data.inputArrayValue(aPositionConstraint);

			//allocate new memory for pPosConVertexIndexList
			dynamicSolverData.pPositionConstraintCoordinateList = new vcsVector [hPositionConstraintArray.elementCount()];
			
			//iterate ArrayElements
			for(unsigned int index = 0; index < hPositionConstraintArray.elementCount(); index++)
			{
				hPositionConstraintArray.jumpToArrayElement(index);
				//get handle to active attr for current array element
				MDataHandle hPositionConstraintArrayElement = hPositionConstraintArray.inputValue();
				MDataHandle hPositionConstraintCoordinate = hPositionConstraintArrayElement.child(aPositionConstraintCoordinate);
				//Create new vcsVector
				float x = hPositionConstraintCoordinate.asFloatVector().x;
				float y = hPositionConstraintCoordinate.asFloatVector().y;
				float z = hPositionConstraintCoordinate.asFloatVector().z;
				dynamicSolverData.pPositionConstraintCoordinateList[index] = vcsVector(x,y,z);
			}
		}
	}
};

//setCollisionConstraintSpheresCountAndVecUpVecDownList
//-----------------------------------------------
void MVerletClothSolver::setCollisionConstraintSpheresCountAndVecUpVecDownList(MDataBlock &data)
{
	//get handle to collision constraint array attr
	MArrayDataHandle hCollisionConstraintCompoundArray = data.inputArrayValue(aCollisionConstraint);
	//elementCount
	int elementCount = hCollisionConstraintCompoundArray.elementCount();
	

	//Iterate to get spheresConstraintCount
	//-----------------------------------------------
	
	//collisionConstraintSpheresCount
	int collisionConstraintSpheresCount = 0;

	 for(int index = 0; index < elementCount; index++)
	{
		hCollisionConstraintCompoundArray.jumpToArrayElement(index);

		//get datahandles to Attrs for current compoundAttr
		MDataHandle hCollisionConstraintCompound = hCollisionConstraintCompoundArray.inputValue();
		short collisionConstraintActive = hCollisionConstraintCompound.child(aCollisionConstraintActive).asShort();
		short collisionConstraintType = hCollisionConstraintCompound.child(aCollisionConstraintType).asShort();
		
		//check collisionConstraintType
		if(collisionConstraintType == 0)
		{
			//check active
			if(collisionConstraintActive)
			{
				//oThisNode
				MObject oThisNode = thisMObject();

				//Get pCollisionConstraintCompoundMatrix
				MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
				MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(index);
				MPlug pCollisionConstraintCompoundMatrix = pCollisionConstraintCompound.child(2);

				//check matrix connected
				if(pCollisionConstraintCompoundMatrix.isConnected())
				{
					//get pCollisionConstraintCompoundGeo
					MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);

					//check geo connected
					if(pCollisionConstraintCompoundGeo.isConnected())
					{
						collisionConstraintSpheresCount++;
					};
				};
			};
		};
	};
	 
	 //set dynamicSolverData
	 dynamicSolverData.collisionConstraintSpheresCount = collisionConstraintSpheresCount;
	 
	 //TMP
	 //MGlobal::displayInfo(MString("CollisionConstraintSpheresCount: ") +collisionConstraintSpheresCount);


	
	 //Iterate to get vecUpvecDownList items
	 //-----------------------------------------------
	 
	 //delete old ptr
	 delete [] dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList;
	 //create new ptr
	 dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList = new vcsVector[collisionConstraintSpheresCount*2];
	
	 //collisionConstraintSpheresIndex
	int collisionConstraintSpheresIndex = 0;

	for(int index = 0; index < elementCount; index ++)
	{
		hCollisionConstraintCompoundArray.jumpToArrayElement(index);

		//get datahandles to Attrs for current compoundAttr
		MDataHandle hCollisionConstraintCompound = hCollisionConstraintCompoundArray.inputValue();
		short collisionConstraintActive = hCollisionConstraintCompound.child(aCollisionConstraintActive).asShort();
		short collisionConstraintType = hCollisionConstraintCompound.child(aCollisionConstraintType).asShort();
		
		//check collisionConstraintType
		if(collisionConstraintType == 0)
		{
			//check active
			if(collisionConstraintActive)
			{
				//oThisNode
				MObject oThisNode = thisMObject();

				//Get pCollisionConstraintCompoundMatrix
				MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
				MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(index);
				MPlug pCollisionConstraintCompoundMatrix = pCollisionConstraintCompound.child(2);

				//check matrix connected
				if(pCollisionConstraintCompoundMatrix.isConnected())
				{
					//get pCollisionConstraintCompoundGeo
					MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);

					//check geo connected
					if(pCollisionConstraintCompoundGeo.isConnected())
					{
						//Get collisionGeo
						MObject collisionGeo = hCollisionConstraintCompound.child(aCollisionConstraintGeo).asMesh();
						//get collisionGeoMatrix
						MMatrix collisionGeoMatrix = hCollisionConstraintCompound.child(aCollisionConstraintGeoMatrix).asMatrix();

						//fnCollisionGeo
						MFnMesh fnCollisionGeo(collisionGeo);
						//collisionGeoPointList
						MPointArray collisionGeoPointList;
						fnCollisionGeo.getPoints(collisionGeoPointList);

						//Get vecUp and vecDown
						int collisionGeoPointListLength = collisionGeoPointList.length();
						MPoint mpVecUp = collisionGeoPointList[collisionGeoPointListLength-1] * collisionGeoMatrix;
						MPoint mpVecDown = collisionGeoPointList[collisionGeoPointListLength-2] * collisionGeoMatrix;

						//cast to vcsVector
						vcsVector vecUp = vcsVector(mpVecUp.x, mpVecUp.y, mpVecUp.z);
						vcsVector vecDown = vcsVector(mpVecDown.x, mpVecDown.y, mpVecDown.z);

						//set dynamicSolverData
						dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionConstraintSpheresIndex] = vecUp;
						dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionConstraintSpheresIndex+1] = vecDown;

						//collisionConstraintSpheresIndex
						collisionConstraintSpheresIndex+=2;

					};
				};

			};
		};

	};
};

//setCollisionConstraintConvexCountAndTriangleCountList
//-----------------------------------------------
void MVerletClothSolver::setCollisionConstraintConvexCountAndTriangleCountList(MDataBlock &data)
{
	//get handle to collision constraint array attr
	MArrayDataHandle hCollisionConstraintCompoundArray = data.inputArrayValue(aCollisionConstraint);
	//elementCount
	int elementCount = hCollisionConstraintCompoundArray.elementCount();
	//oThisNode
	MObject oThisNode = thisMObject();

	//Iterate to get convexConstraintCount
	//-----------------------------------------------

	//collisionConstraintConvexArrayIndexList
	std::vector<int> collisionConstraintConvexArrayIndexList;

	 for(int index = 0; index < elementCount; index++)
	{
		//Jump to array element
		hCollisionConstraintCompoundArray.jumpToArrayElement(index);

		//get datahandles to Attrs for current compoundAttr
		MDataHandle hCollisionConstraintCompound = hCollisionConstraintCompoundArray.inputValue();
		short collisionConstraintActive = hCollisionConstraintCompound.child(aCollisionConstraintActive).asShort();
		short collisionConstraintType = hCollisionConstraintCompound.child(aCollisionConstraintType).asShort();
		
		//check collisionConstraintType
		if(collisionConstraintType == 1)
		{
			//check active
			if(collisionConstraintActive)
			{
				//Get pCollisionConstraintCompoundMatrix
				MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
				MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(index);
				MPlug pCollisionConstraintCompoundMatrix = pCollisionConstraintCompound.child(2);

				//check matrix connected
				if(pCollisionConstraintCompoundMatrix.isConnected())
				{
					//get pCollisionConstraintCompoundGeo
					MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);

					//check geo connected
					if(pCollisionConstraintCompoundGeo.isConnected())
					{
						collisionConstraintConvexArrayIndexList.push_back(index);
					};
				};
			};
		};
	};
	 
	 //collisionConstraintConvexCount
	int collisionConstraintConvexCount = collisionConstraintConvexArrayIndexList.size();

	//set dynamicSolverData convex constraint count
	dynamicSolverData.collisionConstraintConvexCount = collisionConstraintConvexCount;

	 //deallocate memory where current ptr points to
	 delete [] dynamicSolverData.pCollisionConstraintConvexTriangleCountList;
	 //reallocate
	 dynamicSolverData.pCollisionConstraintConvexTriangleCountList = new int[collisionConstraintConvexCount];

	 //Fill triangleCountList
	 for(int index = 0; index < collisionConstraintConvexCount; index++)
	 {
		//arrayIndex
		 int collisionConstraintConvexArrayIndex = collisionConstraintConvexArrayIndexList[index];

		 //Get pCollisionConstraintCompoundGeo
		MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
		MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(collisionConstraintConvexArrayIndex);
		MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);
		
		//triangleCountPerPolygonList & triangleVertexIndexList
		MIntArray triangleCountPerPolygonList;
		MIntArray triangleVertexIndexList;

		//fnMeshCollisionGeo
		MObject oCollisionGeo = pCollisionConstraintCompoundGeo.asMObject();
		MFnMesh fnMeshCollisionGeo(oCollisionGeo);
		
		//get triangles
		fnMeshCollisionGeo.getTriangles(triangleCountPerPolygonList, triangleVertexIndexList);

		//collisionGeoTriangleCount
		int collisionGeoTriangleCount = 0;
		for(int i = 0; i < triangleCountPerPolygonList.length(); i++)
		{
			int triangleCountPerPolygon = triangleCountPerPolygonList[i];
			collisionGeoTriangleCount += triangleCountPerPolygon;
		};

		//set to dynamicSolverData
		dynamicSolverData.pCollisionConstraintConvexTriangleCountList[index] = collisionGeoTriangleCount;
	 };




};

//setCollisionConstraintConvexTriangleVertexPositionList
//-----------------------------------------------
void MVerletClothSolver::setCollisionConstraintConvexTriangleVertexPositionList(MDataBlock &data)
{

	//get handle to collision constraint array attr
	MArrayDataHandle hCollisionConstraintCompoundArray = data.inputArrayValue(aCollisionConstraint);
	//elementCount
	int elementCount = hCollisionConstraintCompoundArray.elementCount();
	//oThisNode
	MObject oThisNode = thisMObject();

	//Iterate to get convexConstraintCount
	//-----------------------------------------------

	//collisionConstraintConvexArrayIndexList
	std::vector<int> collisionConstraintConvexArrayIndexList;

	 for(int index = 0; index < elementCount; index++)
	{
		//Jump to array element
		hCollisionConstraintCompoundArray.jumpToArrayElement(index);

		//get datahandles to Attrs for current compoundAttr
		MDataHandle hCollisionConstraintCompound = hCollisionConstraintCompoundArray.inputValue();
		short collisionConstraintActive = hCollisionConstraintCompound.child(aCollisionConstraintActive).asShort();
		short collisionConstraintType = hCollisionConstraintCompound.child(aCollisionConstraintType).asShort();
		
		//check collisionConstraintType
		if(collisionConstraintType == 1)
		{
			//check active
			if(collisionConstraintActive)
			{
				//Get pCollisionConstraintCompoundMatrix
				MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
				MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(index);
				MPlug pCollisionConstraintCompoundMatrix = pCollisionConstraintCompound.child(2);

				//check matrix connected
				if(pCollisionConstraintCompoundMatrix.isConnected())
				{
					//get pCollisionConstraintCompoundGeo
					MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);

					//check geo connected
					if(pCollisionConstraintCompoundGeo.isConnected())
					{
						collisionConstraintConvexArrayIndexList.push_back(index);
					};
				};
			};
		};
	};
	 
	 //collisionConstraintConvexCount
	int collisionConstraintConvexCount = collisionConstraintConvexArrayIndexList.size();

	//triangleVertexPositionList
	std::vector<vcsVector> triangleVertexPositionList;

	//Fill triangleCountList
	 for(int index = 0; index < collisionConstraintConvexCount; index++)
	 {
		//arrayIndex
		 int collisionConstraintConvexArrayIndex = collisionConstraintConvexArrayIndexList[index];

		 //Get pCollisionConstraintCompoundMatrix & pCollisionConstraintCompoundGeo
		MPlug pCollisionConstraintCompoundArray = MPlug(oThisNode, aCollisionConstraint);
		MPlug pCollisionConstraintCompound = pCollisionConstraintCompoundArray.elementByPhysicalIndex(collisionConstraintConvexArrayIndex);
		MPlug pCollisionConstraintCompoundMatrix = pCollisionConstraintCompound.child(2);
		MPlug pCollisionConstraintCompoundGeo = pCollisionConstraintCompound.child(3);
		
		//transformMatrix
		MObject oTransformMatrix = pCollisionConstraintCompoundMatrix.asMObject();
		//fnMatrixDataTransformMatrix
		MFnMatrixData fnMatrixDataTransformMatrix(oTransformMatrix);
		MMatrix transformMatrix = fnMatrixDataTransformMatrix.matrix();

		//triangleCountPerPolygonList & triangleVertexIndexList
		MIntArray triangleCountPerPolygonList;
		MIntArray triangleVertexIndexList;

		//fnMeshCollisionGeo
		MObject oCollisionGeo = pCollisionConstraintCompoundGeo.asMObject();
		MFnMesh fnMeshCollisionGeo(oCollisionGeo);

		//collisionGeoPointArray
		MPointArray collisionGeoPointArray;
		fnMeshCollisionGeo.getPoints(collisionGeoPointArray);
		
		//get triangles
		fnMeshCollisionGeo.getTriangles(triangleCountPerPolygonList, triangleVertexIndexList);

		//get triangle vertex positions
		for(int triangleIndex = 0; triangleIndex < triangleVertexIndexList.length(); triangleIndex += 3)
		{
			//triangleVertexIndices
			int triangleVertexIndex1 = triangleVertexIndexList[triangleIndex];
			int triangleVertexIndex2 = triangleVertexIndexList[triangleIndex+1];
			int triangleVertexIndex3 = triangleVertexIndexList[triangleIndex+2];

			//get vertexPositions
			MPoint triangleVertexPosition1 = collisionGeoPointArray[triangleVertexIndex1] * transformMatrix;
			MPoint triangleVertexPosition2 = collisionGeoPointArray[triangleVertexIndex2] * transformMatrix;
			MPoint triangleVertexPosition3 = collisionGeoPointArray[triangleVertexIndex3] * transformMatrix;

			//convert to vcsVector
			vcsVector vecTriangleVertexPosition1 = vcsVector(triangleVertexPosition1.x, triangleVertexPosition1.y, triangleVertexPosition1.z);
			vcsVector vecTriangleVertexPosition2 = vcsVector(triangleVertexPosition2.x, triangleVertexPosition2.y, triangleVertexPosition2.z);
			vcsVector vecTriangleVertexPosition3 = vcsVector(triangleVertexPosition3.x, triangleVertexPosition3.y, triangleVertexPosition3.z);
			
			//append to vector
			triangleVertexPositionList.push_back(vecTriangleVertexPosition1);
			triangleVertexPositionList.push_back(vecTriangleVertexPosition2);
			triangleVertexPositionList.push_back(vecTriangleVertexPosition3);
		};


	 };

	 //free memory
	 delete [] dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList;
	 //reallocate
	 dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList = new vcsVector[triangleVertexPositionList.size()];
	
	 //assign values to dynamicSolverData
	 for(int index = 0; index < triangleVertexPositionList.size(); index++)
	 {	
		 dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList[index] = triangleVertexPositionList[index];
	 };

	 //Set collisionConstraintConvexTriangleVertexCount
	 dynamicSolverData.collisionConstraintConvexTriangleVertexCount = triangleVertexPositionList.size();
};

//setCollisionConstraintConvexOffset
//-----------------------------------------------
void MVerletClothSolver::setCollisionConstraintConvexOffset(MDataBlock &data)
{
	//get collisionConstraintConvexOffset from datablock
	float collisionConstraintConvexOffset = data.inputValue(aCollisionConstraintConvexOffset).asFloat();

	//set in dynamicSolverData
	dynamicSolverData.collisionConstraintConvexOffset = collisionConstraintConvexOffset;
};

//set_thread_count
//-----------------------------------------------
void MVerletClothSolver::set_thread_count_and_omp(MDataBlock &data)
{
	//get thread_count from datablock
	int thread_count = data.inputValue(a_thread_count).asInt();
	
	//get omp from datablock
	int omp = static_cast<int> (data.inputValue(a_omp).asShort());

	//set in dynamicSolverData
	dynamicSolverData.thread_count = thread_count;
	//set in dynamicSolverData
	dynamicSolverData.omp = omp;
};


//drawData
//-----------------------------------------------

//setDrawData
//-----------------------------------------------
void MVerletClothSolver::setDrawData(MDataBlock &data)
{
	//Set Data 
	drawData.collisionConstraintGroundPlaneVisible = data.inputValue(aCollisionConstraintGroundplaneVisible).asBool();
	drawData.collisionConstraintGroundplaneDispSize = data.inputValue(aCollisionConstraintGroundplaneDispSize).asFloat();
	drawData.collisionConstraintGroundplaneHeight = data.inputValue(aCollisionConstraintGroundplaneHeight).asFloat();
	drawData.drawMeshlinesActive = data.inputValue(aDrawMeshlinesActive).asBool();
	setVertexColorList(data);
};

//setVertexColorList
//-----------------------------------------------
void MVerletClothSolver::setVertexColorList(MDataBlock &data)
{
	//get colormode
	short colorMode = data.inputValue(aDrawMeshlinesColorMode).asShort();

	//switch
	switch(colorMode)
	{
		case 0:
			setVertexColorListMono();
			break;
		case 1:
			setVertexColorListDistortion();
			break;
		case 2:
			setVertexColorListChange();
			break;
		default:
			break;
	};

}


//setVertexColorListMono
//-----------------------------------------------
void MVerletClothSolver::setVertexColorListMono()
{
	//Delete content of old ptr
	delete [] drawData.pVertexColorList;

	//create new ptr
	drawData.pVertexColorList = new vcsVector[staticSolverData.vertexCount];

	//iterate and set
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		drawData.pVertexColorList[index] = vcsVector(1.0, 0.0, 0.0);
	};
};

//setVertexColorListDistortion
//-----------------------------------------------
void MVerletClothSolver::setVertexColorListDistortion()
{
	//Delete content of old ptr
	delete [] drawData.pVertexColorList;

	//create new ptr
	drawData.pVertexColorList = new vcsVector[staticSolverData.vertexCount];

	
	//get edgeCurrentLengthList
	float* pEdgeCurrentLengthList = new float[staticSolverData.edgeCount];
	//iterate and set
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//vertexIndices
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);

		//get posVectors
		vcsVector vecA = staticSolverData.pVertexPositionList[vertexIndex1];
		vcsVector vecB = staticSolverData.pVertexPositionList[vertexIndex2];

		//get length
		vcsVector vecAB = vecB - vecA;
		float edgeLength = vecAB.length();

		//Set to list
		pEdgeCurrentLengthList[index] = edgeLength;
	};

	
	//get pEdgeDistortionList
	float* pEdgeDistortionList = new float[staticSolverData.edgeCount];
	//iterate and set
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{	
		//get values
		float currentLength = pEdgeCurrentLengthList[index];
		float restLength = staticSolverData.pEdgeRestlengthList[index];

		//set values
		if((currentLength - restLength) < 0){pEdgeDistortionList[index] = (currentLength - restLength) * -1.0;}
		else{pEdgeDistortionList[index] = currentLength - restLength;};
	};

	//get max distortion
	float maxDistortion = 0.0;
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		if(pEdgeDistortionList[index] > maxDistortion){maxDistortion = pEdgeDistortionList[index];};
	};

	

	//iterate and set
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//get color
		float r = 0.0;
		if(maxDistortion > 0.0){r = pEdgeDistortionList[index]/maxDistortion;};
		float g = 0.0;
		float b = 0.0;

		//vertexIndices
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);
		
		//set pVertexColorList
		drawData.pVertexColorList[vertexIndex1] = vcsVector(r, g, b);
		drawData.pVertexColorList[vertexIndex2] = vcsVector(r, g, b);
	};

	//Delete method scope ptr
	delete [] pEdgeCurrentLengthList;
	delete [] pEdgeDistortionList;
};

//setVertexColorListChange
//-----------------------------------------------
void MVerletClothSolver::setVertexColorListChange()
{
	//Delete content of old ptr
	delete [] drawData.pVertexColorList;

	//create new ptr
	drawData.pVertexColorList = new vcsVector[staticSolverData.vertexCount];

	//get edgeChangeLengthList
	float* pEdgeChangeLengthList = new float[staticSolverData.edgeCount];
	//iterate and set
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//vertexIndices
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);

		//get posVectors & posVectorsOld
		vcsVector vecA = staticSolverData.pVertexPositionList[vertexIndex1];
		vcsVector vecAOld = staticSolverData.pVertexOldpositionList[vertexIndex1];
		vcsVector vecB = staticSolverData.pVertexPositionList[vertexIndex2];
		vcsVector vecBOld = staticSolverData.pVertexOldpositionList[vertexIndex2];

		//get lengths
		float vecALength = (vecA - vecAOld).length();
		float vecBLength = (vecB - vecBOld).length();

		//get changelength
		float changeLength = (vecALength + vecBLength) * 0.5;

		//set to list
		pEdgeChangeLengthList[index] = changeLength;
	};

	//get max changeLength
	float maxChangeLength = 0.0;
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		if(pEdgeChangeLengthList[index] > maxChangeLength){maxChangeLength = pEdgeChangeLengthList[index];};
	};

	//iterate and set
	for(int index = 0; index < staticSolverData.edgeCount; index++)
	{
		//get color
		float r = 0.0;
		float g = 0.0;
		if(maxChangeLength > 0.0){g = pEdgeChangeLengthList[index]/maxChangeLength;};
		float b = 0.0;

		//vertexIndices
		int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
		int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);
		
		//set pVertexColorList
		drawData.pVertexColorList[vertexIndex1] = vcsVector(r, g, b);
		drawData.pVertexColorList[vertexIndex2] = vcsVector(r, g, b);
	};

	//delete custom ptr data
	delete [] pEdgeChangeLengthList;
};

//printDrawData
//-----------------------------------------------
void MVerletClothSolver::printDrawData()
{
	
	MGlobal::displayInfo("Draw Data");
	MGlobal::displayInfo("----------------------------------------------");

	//Groundplane Visible
	MGlobal::displayInfo(MString("Groundplane Visible: ") +drawData.collisionConstraintGroundPlaneVisible);
	
	//Groundplane Display Size
	MGlobal::displayInfo(MString("Groundplane Display Size: ") +drawData.collisionConstraintGroundplaneDispSize);

	//Groundplane Height
	MGlobal::displayInfo(MString("Groundplane Height: ") +drawData.collisionConstraintGroundplaneHeight);

	//Draw Meshlines Active
	MGlobal::displayInfo(MString("Draw Meshlines Active: ") +drawData.drawMeshlinesActive);

	//vertexColorList
	MGlobal::displayInfo("vertexColorList:");
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float r = drawData.pVertexColorList[index].x;
		float g = drawData.pVertexColorList[index].y;
		float b = drawData.pVertexColorList[index].z;
		MString msgIndex = MString("Index: ") +index;
		MString msgR = MString(" R: ") +r;
		MString msgG = MString(" G: ") +g;
		MString msgB = MString(" B: ") +b;
		MGlobal::displayInfo(msgIndex +msgR +msgG + msgB);
	};

};




//MVerletClothSolver Methods
//-----------------------------------------------

//attributeConnected
//-----------------------------------------------
bool MVerletClothSolver::attributeConnected(MString attributeName)
{
	//fs for this node
	MFnDependencyNode fsDepNode(thisMObject());
	//Get plug to attr
	MPlug pAttr = fsDepNode.findPlug(attributeName, true);
	
	return pAttr.isConnected();
};

//setOutputGeo
//-----------------------------------------------
void MVerletClothSolver::setOutputGeo(MDataBlock &data)
{
	//Get vertexPositionList
	MPointArray vertexPositionList(staticSolverData.vertexCount);
	
	for(int index = 0; index < staticSolverData.vertexCount; index++)
	{
		float x = staticSolverData.pVertexPositionList[index].x;
		float y = staticSolverData.pVertexPositionList[index].y;
		float z = staticSolverData.pVertexPositionList[index].z;
		vertexPositionList.set(MPoint(x,y,z), index);
	}

	//Get fsInputGeo
	MObject oInputGeo = data.inputValue(aInputGeo).asMesh();
	MFnMesh fsInputGeo(oInputGeo);

	//Get polyCount and polyConnect Arrays
	MIntArray polyCountArray;
	MIntArray polyConnectArray;
	fsInputGeo.getVertices(polyCountArray, polyConnectArray);

	//Create MeshDataBlock
	MFnMeshData fsMeshData;
	MObject oMeshDataBlock = fsMeshData.create();

	//Create new geo
	MFnMesh fsOutputGeo;
	MObject oOutputGeo = fsOutputGeo.create(staticSolverData.vertexCount, polyCountArray.length(), vertexPositionList, polyCountArray, polyConnectArray, oMeshDataBlock);

	//set outputGeo in datablock
	MDataHandle hOutputGeo = data.outputValue(aOutputGeo);
	hOutputGeo.setMObject(oMeshDataBlock);
	
};

//getUiFps
//-----------------------------------------------
float MVerletClothSolver::getUiFps()
{
	MTime::Unit uiFps = MTime::uiUnit();
	
	switch(uiFps)
	{
	case MTime::Unit::kFilm:
		return 24.0;
	case MTime::Unit::kPALFrame:
		return 25.0;
	case MTime::Unit::kPALField:
		return 50.0;
	case MTime::Unit::kNTSCFrame:
		return 30.0;
	case MTime::Unit::kNTSCField:
		return 60.0;
	case MTime::Unit::k12FPS:
		return 12.0;
	case MTime::Unit::k10FPS:
		return 10.0;
	default:return 1.0;
	};
};




//TEMP
//-----------------------------------------------

