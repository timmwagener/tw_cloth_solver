

#ifndef MVERLETCLOTHSOLVER_H
#define MVERLETCLOTHSOLVER_H

//MVerletClothSolver Declaration
//-----------------------------------------------


//include
//-----------------------------------------------
#include <maya/MPxLocatorNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MFloatVector.h>
#include <maya/MIntArray.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MTime.h>
#include <maya/MItMeshEdge.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MFnMatrixData.h>
#include <maya/MItMeshPolygon.h>

#include <vector>
#include <set>

#include "verletClothSolverPlugin.h"



//MVerletClothSolver
//-----------------------------------------------
class MVerletClothSolver: public MPxLocatorNode, public VerletClothSolverPlugin
{
public:
	//Attributes
	static MTypeId id;
	static MString typeName;

	static MObject aVerbose;
	static MObject aVerboseStaticSolverData;
	static MObject aVerboseDynamicSolverData;
	static MObject aVerboseDrawData;

	static MObject aSolverType;

	static MObject aInputGeo;
	static MObject aOutputGeo;

	static MObject aStartFrame;
	static MObject aCurrentFrame;

	static MObject aGravity;

	static MObject aTransformMatrix;
	
	static MObject aDrag;

	static MObject aCollisionConstraintGroundplaneActive;
    static MObject aCollisionConstraintGroundplaneHeight;
	static MObject aCollisionConstraintGroundplaneVisible;
	static MObject aCollisionConstraintGroundplaneDispSize;

	static MObject aCollisionConstraint;
	static MObject aCollisionConstraintActive;
	static MObject aCollisionConstraintType;
	static MObject aCollisionConstraintGeoMatrix;
	static MObject aCollisionConstraintGeo;
	
	static MObject aCollisionConstraintConvexOffset;

	static MObject aRepetitions;

	static MObject aPositionConstraint;
	static MObject aPositionConstraintActive;
	static MObject aPositionConstraintVertexIndex;
	static MObject aPositionConstraintCoordinate;

	static MObject aDrawMeshlinesActive;
	static MObject aDrawMeshlinesColorMode;

	static MObject aSolveTriangulated;

	static MObject a_thread_count;
	static MObject a_omp;

	//Methods
	MVerletClothSolver();
	virtual ~MVerletClothSolver();
	static void* create();
	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	virtual void draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus);
	
	
	//staticSolverData
	void setStaticSolverData(void* proprietaryData);
	void printStaticSolverData();
	void setVertexCount(MDataBlock &data);
	void setVertexPositionList(MDataBlock &data);
	void setVertexOldpositionList(MDataBlock &data);
	void setVertexInitialpositionList(MDataBlock &data);
	void setEdgeVertexIndexListAndCount(MDataBlock &data);
	void setEdgeRestlengthList(MDataBlock &data);
	
	//dynamicSolverData
	void setDynamicSolverData(void* proprietaryData);
	void printDynamicSolverData();
	void setVertexForceList(MDataBlock &data);
	void setPositionConstraintCount(MDataBlock &data);
	void setPositionConstraintActiveList(MDataBlock &data);
	void setPositionConstraintVertexIndexList(MDataBlock &data);
	void setPositionConstraintCoordinateList(MDataBlock &data);
	void setCollisionConstraintSpheresCountAndVecUpVecDownList(MDataBlock &data);
	void setCollisionConstraintConvexCountAndTriangleCountList(MDataBlock &data);
	void setCollisionConstraintConvexTriangleVertexPositionList(MDataBlock &data);
	void setCollisionConstraintConvexOffset(MDataBlock&);
	void set_thread_count_and_omp(MDataBlock&);

	//drawData
	void setDrawData(MDataBlock &data);
	void printDrawData();
	void setVertexColorList(MDataBlock &data);
	void setVertexColorListMono();
	void setVertexColorListDistortion();
	void setVertexColorListChange();

	//draw
	void drawCollisionGroundplane(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus);
	void drawMeshlines(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus);
	
	//MVerletClothSolver
	float getUiFps();
	void setOutputGeo(MDataBlock &data);
	bool attributeConnected(MString attributeName);

	//temp


private:
	//Attributes
	float lastFrame;
	drawData_t drawData;

	//Methods


};
#endif