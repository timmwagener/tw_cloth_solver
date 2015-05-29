

#ifndef TESSELATIONVISUALIZATION_H
#define TESSELATIONVISUALIZATION_H

//TesselationVisualization Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include <vector>
#include <set>

#include <maya/MPxLocatorNode.h>
#include <maya/MObject.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MPoint.h>




//drawData_t
//-----------------------------------------------
struct tesselationDrawData_t
{
	//Attrs
	std::vector<std::vector<float>> edgeVertexPositionList;
	//Methods
	tesselationDrawData_t(){};
	~tesselationDrawData_t(){};
};


//TesselationVisualization
//-----------------------------------------------
class TesselationVisualization: public MPxLocatorNode
{
public:
	//Attributes
	static MTypeId id;
	static MString typeName;

	static MObject aVerbose;

	static MObject aTesselationType;
	
	static MObject aInputGeo;
	static MObject aOutputGeo;

	static MObject aCurrentFrame;

	//Methods
	TesselationVisualization();
	virtual ~TesselationVisualization();
	static void* create();
	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	virtual void draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus);

	//drawData
	void setTesselationDrawData(MDataBlock &data);
	void setEdgeVertexPositionList(MDataBlock &data);
	void setEdgeVertexPositionListStructural(MDataBlock &data);
	void setEdgeVertexPositionListShear(MDataBlock &data);

private:
	//Attributes
	tesselationDrawData_t drawData;

	//Methods
};

#endif