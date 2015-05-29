

//TesselationVisualization Definition
//-----------------------------------------------


//include
//-----------------------------------------------
#include "tesselationVisualization.h"





//TesselationVisualization
//-----------------------------------------------

//Attributes
MTypeId TesselationVisualization::id(0x70007);
MString TesselationVisualization::typeName("tesselationVisualization");

MObject TesselationVisualization::aVerbose;

MObject TesselationVisualization::aTesselationType;

MObject TesselationVisualization::aInputGeo;
MObject TesselationVisualization::aOutputGeo;

MObject TesselationVisualization::aCurrentFrame;

//Methods
TesselationVisualization::TesselationVisualization(){};
TesselationVisualization::~TesselationVisualization(){};

//create
//-----------------------------------------------
void* TesselationVisualization::create()
{
	return new TesselationVisualization();
};

//initialize
//-----------------------------------------------
MStatus TesselationVisualization::initialize()
{
	
	//Attr functionsets
	MFnEnumAttribute eAttr;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	//aVerbose
	aVerbose = eAttr.create("verbose", "verbose", 0);
	eAttr.addField("noVerbose", 0);
	eAttr.addField("Verbose", 1);
	addAttribute(aVerbose);

	//aTesselationType
	aTesselationType = eAttr.create("tesselationType", "tesselationType", 0);
	eAttr.addField("Structural", 0);
	eAttr.addField("Shear", 1);
	addAttribute(aTesselationType);
	
	//aInputGeo
	aInputGeo = tAttr.create("inputGeo", "inputGeo", MFnData::kMesh);
	tAttr.setReadable(false);
	addAttribute(aInputGeo);
	
	//aOutputGeo
	aOutputGeo = tAttr.create("outputGeo", "outputGeo", MFnData::kMesh);
	tAttr.setWritable(false);
	tAttr.setStorable(false);
	addAttribute(aOutputGeo);

	//aCurrentFrame
	aCurrentFrame = nAttr.create("currentFrame", "currentFrame", MFnNumericData::kFloat);
	addAttribute(aCurrentFrame);

	//AttributeAffects
	attributeAffects(aTesselationType, aOutputGeo);
	attributeAffects(aInputGeo, aOutputGeo);
	attributeAffects(aCurrentFrame, aOutputGeo);

	return MStatus::kSuccess;
};

//compute
//-----------------------------------------------
MStatus TesselationVisualization::compute(const MPlug &plug, MDataBlock &data)
{
	if(plug != aOutputGeo)
	{
		MGlobal::displayInfo("Requested Plug is not for aOutputGeo");
	}
	else
	{
		//verbose
		short verbose = data.inputValue(aVerbose).asShort();

		//Set tesselationDrawData
		setTesselationDrawData(data);

		MGlobal::displayInfo("Compute");
		
	};
	
	return MStatus::kSuccess;
};

//draw
//-----------------------------------------------
void TesselationVisualization::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus)
{
	MString msg = MString("DrawMethod");
	MGlobal::displayInfo(msg);

	if(!drawData.edgeVertexPositionList.size())
	{
		MGlobal::displayInfo("edgeVertexPositionList empty");
		return;
	};

	//Get glFT
	static MGLFunctionTable* pGlFT = 0;
	if ( 0 == pGlFT ){pGlFT = MHardwareRenderer::theRenderer()->glFunctionTable();};

	// Begin the drawing
	view.beginGL();

	//Store current settings
	pGlFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);
	
	pGlFT->glEnable(GL_POINT_SMOOTH);
	pGlFT->glPointSize(5.0);

	for(int index = 0; index < drawData.edgeVertexPositionList.size(); index++)
	{
		////Draw Lines
		//pGlFT->glBegin(MGL_POINTS);
		//
		////Color
		//pGlFT->glColor3f(1.0f, 1.0f,0.0f);

		////positions
		//float posX1 = drawData.edgeVertexPositionList[index][0];
		//float posY1 = drawData.edgeVertexPositionList[index][1];
		//float posZ1 = drawData.edgeVertexPositionList[index][2];

		//float posX2 = drawData.edgeVertexPositionList[index][3];
		//float posY2 = drawData.edgeVertexPositionList[index][4];
		//float posZ2 = drawData.edgeVertexPositionList[index][5];

		////draw line
		//pGlFT->glVertex3f( posX1, posY1, posZ1);
		//pGlFT->glVertex3f( posX2, posY2, posZ2);
		//
		////End Lines
		//pGlFT->glEnd();
		

		//Draw Lines
		pGlFT->glBegin(MGL_LINES);
		//Color
		pGlFT->glColor3f(1.0f, 1.0f,0.0f);

		//positions
		float posX1 = drawData.edgeVertexPositionList[index][0];
		float posY1 = drawData.edgeVertexPositionList[index][1];
		float posZ1 = drawData.edgeVertexPositionList[index][2];

		float posX2 = drawData.edgeVertexPositionList[index][3];
		float posY2 = drawData.edgeVertexPositionList[index][4];
		float posZ2 = drawData.edgeVertexPositionList[index][5];

		//draw line
		pGlFT->glVertex3f( posX1, posY1, posZ1);
		pGlFT->glVertex3f( posX2, posY2, posZ2);
		
		//End Lines
		pGlFT->glEnd();
	}

	//Reset current settings
	pGlFT->glPopAttrib();

	view.endGL();

};

//setDrawData
//-----------------------------------------------
void TesselationVisualization::setTesselationDrawData(MDataBlock &data)
{
	//setEdgeVertexIndexList
	setEdgeVertexPositionList(data);
};



//setEdgeVertexIndexList
//-----------------------------------------------
void TesselationVisualization::setEdgeVertexPositionList(MDataBlock &data)
{
	
	//get colormode
	short tesselationType = data.inputValue(aTesselationType).asShort();

	//switch
	switch(tesselationType)
	{
		case 0:
			setEdgeVertexPositionListStructural(data);
			break;
		case 1:
			setEdgeVertexPositionListShear(data);
			break;
		default:
			break;
	};
};

//setEdgeVertexIndexListStructural
//-----------------------------------------------
void TesselationVisualization::setEdgeVertexPositionListStructural(MDataBlock &data)
{	
	//clear edgeVertexPositionList
	drawData.edgeVertexPositionList.clear();

	//inputGeo
	MObject oInputGeo = data.inputValue(aInputGeo).asMesh();

	//fnMeshInputGeo
	MFnMesh fnMeshInputGeo(oInputGeo);

	//vertexPositionList
	MPointArray vertexPositionList;
	fnMeshInputGeo.getPoints(vertexPositionList);

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
			fnMeshInputGeo.getEdgeVertices(edgeIndexList[index], edgeVertexIndices[index]);
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


		//get faceVertexList
		MPointArray faceVertexPointList = MPointArray(4);

		for(int index = 0; index < edgeIndexList.length(); index++)
		{
			MPoint faceVertexPoint = vertexPositionList[vecEdgeVertexIndicesNoDuplicates[index]];
			faceVertexPointList.set(faceVertexPoint, index);
		};

		
		
		//edge01
		std::vector<float> edge01;
		edge01.push_back(faceVertexPointList[0].x);edge01.push_back(faceVertexPointList[0].y);edge01.push_back(faceVertexPointList[0].z);
		edge01.push_back(faceVertexPointList[1].x);edge01.push_back(faceVertexPointList[1].y);edge01.push_back(faceVertexPointList[1].z);

		//edge13
		std::vector<float> edge13;
		edge13.push_back(faceVertexPointList[1].x);edge13.push_back(faceVertexPointList[1].y);edge13.push_back(faceVertexPointList[1].z);
		edge13.push_back(faceVertexPointList[3].x);edge13.push_back(faceVertexPointList[3].y);edge13.push_back(faceVertexPointList[3].z);

		//edge32
		std::vector<float> edge32;
		edge32.push_back(faceVertexPointList[3].x);edge32.push_back(faceVertexPointList[3].y);edge32.push_back(faceVertexPointList[3].z);
		edge32.push_back(faceVertexPointList[2].x);edge32.push_back(faceVertexPointList[2].y);edge32.push_back(faceVertexPointList[2].z);

		//edge20
		std::vector<float> edge20;
		edge20.push_back(faceVertexPointList[2].x);edge20.push_back(faceVertexPointList[2].y);edge20.push_back(faceVertexPointList[2].z);
		edge20.push_back(faceVertexPointList[0].x);edge20.push_back(faceVertexPointList[0].y);edge20.push_back(faceVertexPointList[0].z);

		
		//add to drawData.edgeVertexPositionList
		drawData.edgeVertexPositionList.push_back(edge01);
		drawData.edgeVertexPositionList.push_back(edge13);
		drawData.edgeVertexPositionList.push_back(edge32);
		drawData.edgeVertexPositionList.push_back(edge20);
		
		//del edgeVertexIndices
		delete [] edgeVertexIndices;
		
		//Advance
		itMeshPolyInputGeo.next();
	};
};

//setEdgeVertexIndexListShear
//-----------------------------------------------
void TesselationVisualization::setEdgeVertexPositionListShear(MDataBlock &data)
{
	//clear edgeVertexPositionList
	drawData.edgeVertexPositionList.clear();

	//inputGeo
	MObject oInputGeo = data.inputValue(aInputGeo).asMesh();

	//fnMeshInputGeo
	MFnMesh fnMeshInputGeo(oInputGeo);

	//vertexPositionList
	MPointArray vertexPositionList;
	fnMeshInputGeo.getPoints(vertexPositionList);

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
			fnMeshInputGeo.getEdgeVertices(edgeIndexList[index], edgeVertexIndices[index]);
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


		//get faceVertexList
		MPointArray faceVertexPointList = MPointArray(4);

		for(int index = 0; index < edgeIndexList.length(); index++)
		{
			MPoint faceVertexPoint = vertexPositionList[vecEdgeVertexIndicesNoDuplicates[index]];
			faceVertexPointList.set(faceVertexPoint, index);
		};

		
		
		//edge01
		std::vector<float> edge01;
		edge01.push_back(faceVertexPointList[0].x);edge01.push_back(faceVertexPointList[0].y);edge01.push_back(faceVertexPointList[0].z);
		edge01.push_back(faceVertexPointList[1].x);edge01.push_back(faceVertexPointList[1].y);edge01.push_back(faceVertexPointList[1].z);

		//edge13
		std::vector<float> edge13;
		edge13.push_back(faceVertexPointList[1].x);edge13.push_back(faceVertexPointList[1].y);edge13.push_back(faceVertexPointList[1].z);
		edge13.push_back(faceVertexPointList[3].x);edge13.push_back(faceVertexPointList[3].y);edge13.push_back(faceVertexPointList[3].z);

		//edge32
		std::vector<float> edge32;
		edge32.push_back(faceVertexPointList[3].x);edge32.push_back(faceVertexPointList[3].y);edge32.push_back(faceVertexPointList[3].z);
		edge32.push_back(faceVertexPointList[2].x);edge32.push_back(faceVertexPointList[2].y);edge32.push_back(faceVertexPointList[2].z);

		//edge20
		std::vector<float> edge20;
		edge20.push_back(faceVertexPointList[2].x);edge20.push_back(faceVertexPointList[2].y);edge20.push_back(faceVertexPointList[2].z);
		edge20.push_back(faceVertexPointList[0].x);edge20.push_back(faceVertexPointList[0].y);edge20.push_back(faceVertexPointList[0].z);

		//edge03
		std::vector<float> edge03;
		edge03.push_back(faceVertexPointList[0].x);edge03.push_back(faceVertexPointList[0].y);edge03.push_back(faceVertexPointList[0].z);
		edge03.push_back(faceVertexPointList[3].x);edge03.push_back(faceVertexPointList[3].y);edge03.push_back(faceVertexPointList[3].z);

		//edge12
		std::vector<float> edge12;
		edge12.push_back(faceVertexPointList[1].x);edge12.push_back(faceVertexPointList[1].y);edge12.push_back(faceVertexPointList[1].z);
		edge12.push_back(faceVertexPointList[2].x);edge12.push_back(faceVertexPointList[2].y);edge12.push_back(faceVertexPointList[2].z);

		
		//add to drawData.edgeVertexPositionList
		drawData.edgeVertexPositionList.push_back(edge01);
		drawData.edgeVertexPositionList.push_back(edge13);
		drawData.edgeVertexPositionList.push_back(edge32);
		drawData.edgeVertexPositionList.push_back(edge20);
		drawData.edgeVertexPositionList.push_back(edge03);
		drawData.edgeVertexPositionList.push_back(edge12);
		
		//del edgeVertexIndices
		delete [] edgeVertexIndices;
		
		//Advance
		itMeshPolyInputGeo.next();
	};
};