


#twClothSolverIOPaperBlockout Module
#-------------------------------------------------------------------------------------


#Last Modified: 18.05.13
#Author: Timm Wagener
#Description: Simple cloth solver implementation



#Imports
#-------------------------------------------------------------------------------------
import sys, math, time, random
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim



#TwClothSolverIOPaperBlockout class
#-------------------------------------------------------------------------------------

class TwClothSolverIOPaperBlockout(OpenMayaMPx.MPxNode):
    '''TwClothSolverIOPaperBlockout class'''


    
    #Class Variables
    #-------------------------------------------------------------------------------------
    
    aInputGeo = OpenMaya.MObject()
    aOutputGeo = OpenMaya.MObject() 
    
    aStartFrame = OpenMaya.MObject()
    aCurrentFrame = OpenMaya.MObject()

    aGravity = OpenMaya.MObject()

    aVertexPositionList = OpenMaya.MObject()
    aVertexOldpositionList = OpenMaya.MObject()
    aVertexInitialpositionList = OpenMaya.MObject()
    
    aEdgeVertexIndexList = OpenMaya.MObject()
    aEdgeRestlengthList = OpenMaya.MObject()

    aVertexForceList = OpenMaya.MObject()

    aTransformMatrix = OpenMaya.MObject()

    aVerbose = OpenMaya.MObject()

    aRepetitions = OpenMaya.MObject()
    
    aPositionConstraint = OpenMaya.MObject()
    aPositionConstraintActive = OpenMaya.MObject()
    aPositionConstraintVertexIndex = OpenMaya.MObject()
    aPositionConstraintCoordinate = OpenMaya.MObject()

    aCollisionConstraint = OpenMaya.MObject()
    aCollisionConstraintActive = OpenMaya.MObject()
    aCollisionConstraintType = OpenMaya.MObject()
    aCollisionConstraintGeoMatrix = OpenMaya.MObject()
    aCollisionConstraintGeo = OpenMaya.MObject()

    aCollisionConstraintGroundplaneActive = OpenMaya.MObject()
    aCollisionConstraintGroundplaneHeight = OpenMaya.MObject()

    
    
    #Methods
    #-------------------------------------------------------------------------------------
    
    def __init__(self):
        '''TwClothSolverIOPaperBlockout __init__'''

        #Execute superclass constructor
        OpenMayaMPx.MPxNode.__init__(self)

        #instance variables
        self.verbose = False


    def compute(self, plug, data):
        '''Compute new value if requested plug is outputGeo'''
        
        #Get all attrs to force update
        inputGeo = data.inputValue(self.aInputGeo).asMesh()

        startFrame = data.inputValue(self.aStartFrame).asInt()
        currentFrame = data.inputValue(self.aCurrentFrame).asFloat()

        gravity = OpenMaya.MFloatVector()
        gravity = data.inputValue(self.aGravity).asFloatVector()

        oVertexPositionList = OpenMaya.MObject()
        oVertexPositionList = data.inputValue(self.aVertexPositionList).data()

        oVertexOldpositionList = OpenMaya.MObject()
        oVertexOldpositionList = data.inputValue(self.aVertexOldpositionList).data()

        oVertexInitialpositionList = OpenMaya.MObject()
        oVertexInitialpositionList = data.inputValue(self.aVertexInitialpositionList).data()

        oEdgeVertexIndexList = OpenMaya.MObject()
        oEdgeVertexIndexList = data.inputValue(self.aEdgeVertexIndexList).data()


        #Check if requested dirty plug equals output plug
        if(plug != self.aOutputGeo): 
            if(self.verbose):OpenMaya.MGlobal.displayInfo('Unknown Plug requested. Compute not executed')
            return OpenMaya.MStatus.kUnknownParameter
        
        
        else:

            #1-set verbose
            self.setVerbose(data)
            if(self.verbose):OpenMaya.MGlobal.displayInfo('-------------ComputationLoop-------------\nVerbose status set')
            
            
            #2-Check if attributes are connected
            #inputGeo
            if not(self.attrConnected('inputGeo')):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('inputGeo not connected')
                return OpenMaya.MStatus.kSuccess
            #transformMatrix
            if not(self.attrConnected('transformMatrix')):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('transformMatrix not connected')
                return OpenMaya.MStatus.kSuccess
            
            
            #3-If currentFrame < startFrame then do nothing
            if(int(currentFrame) < startFrame):
                self.setVertexPositionListsForStartFrame(data)
                self.setEdgeVertexIndexListForStartFrame(data)
                self.setEdgeRestlengthListForStartFrame(data)
                self.draw(data)
                data.setClean(plug)
                if(self.verbose):OpenMaya.MGlobal.displayInfo('currentFrame < startFrame')
                return OpenMaya.MStatus.kSuccess

            
            #4-If currentFrame == startFrame then set vertexPositionLists and edgeVertexIndexList
            if(int(currentFrame) == startFrame):
                self.setVertexPositionListsForStartFrame(data)
                self.setEdgeVertexIndexListForStartFrame(data)
                self.setEdgeRestlengthListForStartFrame(data)
                self.draw(data)
                data.setClean(plug)
                if(self.verbose):OpenMaya.MGlobal.displayInfo('currentFrame == startFrame. Set vertexList attributes')
                return OpenMaya.MStatus.kSuccess


            #5-If currentFrame > startFrame then do timestep
            if(int(currentFrame) > startFrame):
                self.accumulateForces(data)
                self.verletIntegration(data)
                self.satisfyConstraints(data, self.getRepetitions(data))
                self.draw(data)
                data.setClean(plug)
                if(self.verbose):OpenMaya.MGlobal.displayInfo('currentFrame > startFrame. Timestepping...')
                return OpenMaya.MStatus.kSuccess

            
            if(self.verbose):OpenMaya.MGlobal.displayInfo('Compute executed')
            return OpenMaya.MStatus.kSuccess

            



    
    #Utility methods
    #-------------------------------------------------------------------------------------

    def setVerbose(self, data):
        '''Set instance variable verbose for output messages'''

        if(data.inputValue(self.aVerbose).asInt()): self.verbose = True
        else:self.verbose = False

    
    
    def attrConnected(self, attrName):
        '''Check if given attr of node is connected'''

        #Create FN for this node
        thisNode = self.thisMObject()
        fnDepNode = OpenMaya.MFnDependencyNode(thisNode)
        #Get MPLug for attribute name
        try:
            pAttr = fnDepNode.findPlug(attrName, True)
        except:
            if(self.verbose): OpenMaya.MGlobal.displayInfo('Error getting plug to attribute')
            return None
        
        return pAttr.isConnected()

    
    
    def setVertexPositionListsForStartFrame(self, data):
        '''Sets VertexPositionLists for startFrame'''

        
        #get transformMatrix
        transformMatrix = data.inputValue(self.aTransformMatrix).asMatrix()
        
        #get inputGeo as MObject
        inputGeo = data.inputValue(self.aInputGeo).asMesh()
        #fsMesh for inputGeo
        fsInputGeo = OpenMaya.MFnMesh(inputGeo)
        #get vertexList
        vertexList = OpenMaya.MPointArray()
        fsInputGeo.getPoints(vertexList)

        
        #set points in vertexList to worldSpace
        for index in range(vertexList.length()):
            vertexList.set(vertexList[index]*transformMatrix, index)
        
        #set attributes in datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexList)
        self.setMpointArrayToDatablock(data, self.aVertexOldpositionList, vertexList)
        self.setMpointArrayToDatablock(data, self.aVertexInitialpositionList, vertexList)

        


    
    def setEdgeVertexIndexListForStartFrame(self, data):
        '''Set Edge-VertexIndex List for startFrame'''

        #get inputGeo as MObject
        inputGeo = data.inputValue(self.aInputGeo).asMesh()
        #fsMesh for inputGeo
        fsInputGeo = OpenMaya.MFnMesh(inputGeo)

        #create MItEdge
        itMeshEdges = OpenMaya.MItMeshEdge(inputGeo)
        util = OpenMaya.MScriptUtil()
        dummyPoint = OpenMaya.MPoint()
        edgeVertexIndexList = OpenMaya.MPointArray(itMeshEdges.count(), dummyPoint)

        while not(itMeshEdges.isDone()):
            
            #get vertex indices for edgeIndex
            util.createFromDouble(0.0)
            ptr = util.asInt2Ptr()
            fsInputGeo.getEdgeVertices(itMeshEdges.index(), ptr)
                
            vertexIndex1 = util.getInt2ArrayItem(ptr, 0,0)
            vertexIndex2 = util.getInt2ArrayItem(ptr, 0,1)

            #set MPoint with first two values as vertexindices and index as edgeIndex in edgeVertexIndexList
            pointVertexIndices = OpenMaya.MPoint(vertexIndex1, vertexIndex2, 0)
            edgeVertexIndexList.set(pointVertexIndices, itMeshEdges.index())

            itMeshEdges.next()
        
        
        #set aEdgeVertexIndexList
        self.setMpointArrayToDatablock(data, self.aEdgeVertexIndexList, edgeVertexIndexList)


    
    def setEdgeRestlengthListForStartFrame(self, data):
        '''Set aEdgeRestlengthList at startFrame'''

        #getEdgeVertexIndexList
        edgeVertexIndexList = self.getMpointArrayFromDatablock(data, self.aEdgeVertexIndexList)
        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)

        #edgeRestlengthList
        edgeRestlengthList = OpenMaya.MPointArray(edgeVertexIndexList)

        #fill edgeRestlengthList
        for index in range(edgeVertexIndexList.length()):
            #get vertexIndices
            vertexIndex1 = int(edgeVertexIndexList[index].x)
            vertexIndex2 = int(edgeVertexIndexList[index].y)
            #get vector from mpoints of indices
            vec1 = OpenMaya.MVector(vertexPositionList[vertexIndex1])
            vec2 = OpenMaya.MVector(vertexPositionList[vertexIndex2])
            #restlength
            restlength = (vec2 - vec1).length()

            #set rl in edgeRestlengthList
            edgeRestlengthList.set(OpenMaya.MPoint(restlength, 0, 0),index)

        #set aEdgeRestlengthList
        self.setMpointArrayToDatablock(data, self.aEdgeRestlengthList, edgeRestlengthList)



    def accumulateForces(self, data):
        '''Set vertexForceList'''

        #get gravity from datablock
        gravity = OpenMaya.MFloatVector()
        gravity = data.inputValue(self.aGravity).asFloatVector()

        #get vertexPositionList from datablock
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)

        #vertexPositionList to vertexForceList
        vertexForceList = OpenMaya.MPointArray(vertexPositionList)

        #set force for each point in vertexForceList
        for index in range(vertexForceList.length()):
            vertexForceList.set(OpenMaya.MPoint(gravity.x, gravity.y, gravity.z), index)

        #set aVertexForceList
        self.setMpointArrayToDatablock(data, self.aVertexForceList, vertexForceList)



    def verletIntegration(self, data):
        '''Move points according to implicit velocity and force'''

        #get oldPos, Pos and forceList from datablock
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)
        vertexOldpositionList = self.getMpointArrayFromDatablock(data, self.aVertexOldpositionList)
        vertexForceList = self.getMpointArrayFromDatablock(data, self.aVertexForceList)


        #verletIntegration for each point
        for index in range(vertexPositionList.length()):
            #get variables for formula
            vecNew = OpenMaya.MVector()
            vecPos = OpenMaya.MVector(vertexPositionList[index])
            vecOldpos = OpenMaya.MVector(vertexOldpositionList[index])
            vecForce = OpenMaya.MVector(vertexForceList[index])
            #verlet integration
            vecNew = ((vecPos * 2) - vecOldpos) + vecForce

            #set new values in lists
            vertexOldpositionList.set(OpenMaya.MPoint(vecPos), index)
            vertexPositionList.set(OpenMaya.MPoint(vecNew), index)

        

        #set vertexPositionLists in datablock
        self.setMpointArrayToDatablock(data, self.aVertexOldpositionList, vertexOldpositionList)
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)
        

    
    def satisfyConstraints(self, data, repetitions):
        '''Run all given constraints in a loop'''

        for index in range(repetitions):
            if(self.getCollisionConstraintGroundplaneActive(data)): self.collisionConstraintGroundplane(data)
            self.collisionConstraint(data)
            self.stickConstraint(data)
            self.positionConstraint(data)
            
            

    
    def collisionConstraintGroundplane(self, data):
        '''Simple collision with a groundplane'''

        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)

        #get groundplaneHeight
        groundplaneHeight = data.inputValue(self.aCollisionConstraintGroundplaneHeight).asFloat()

        #iterate vertexPositionList and set y when value < groundplaneHeight
        for index in range(vertexPositionList.length()):
            if(vertexPositionList[index].y < groundplaneHeight): vertexPositionList.set(OpenMaya.MPoint(vertexPositionList[index].x, groundplaneHeight, vertexPositionList[index].z) ,index)

        #set vertexPositionList to datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)


    
    def collisionConstraint(self, data):
        '''Collsion constraint detects if points intersect objects and if so project them outward'''

        #thisNode MObject
        thisNode = self.thisMObject()

        #handle to collisionConstraint array attr
        haCollisionConstraintArray = data.inputArrayValue(self.aCollisionConstraint)

        #iterate all array elements
        for index in range(haCollisionConstraintArray.elementCount()):
            haCollisionConstraintArray.jumpToArrayElement(index)
            #get datahandle to active attr for current array element
            hCollisionConstraintArrayElement = haCollisionConstraintArray.inputValue()
            hCollisionConstraintActive = hCollisionConstraintArrayElement.child(self.aCollisionConstraintActive)
            
            #if current collisionConstraint is active check collisionType
            if(hCollisionConstraintActive.asInt()):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint at index %s active' % (index))
                
                #get plug to current collisionConstraint compund array attr
                pCollisionConstraintCompoundArrayAttr = OpenMaya.MPlug(thisNode, self.aCollisionConstraint)
                pCollisionConstraintCompoundAttr = pCollisionConstraintCompoundArrayAttr.elementByPhysicalIndex(index)

                #check if current collision geo matrix connected
                pCollisionConstraintGeoMatrixAttr = pCollisionConstraintCompoundAttr.child(2)
                if(pCollisionConstraintGeoMatrixAttr.isConnected()):
                    if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint Geo Matrix at index %s connected' % (index))

                    #check if current collision geo connected
                    pCollisionConstraintGeoAttr = pCollisionConstraintCompoundAttr.child(3)
                    if(pCollisionConstraintGeoAttr.isConnected()):
                        if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint Geo at index %s connected' % (index))
                    
                        #get collision geo
                        collisionGeo = hCollisionConstraintArrayElement.child(self.aCollisionConstraintGeo).asMesh()
                        #get collision geo matrix
                        collisionGeoMatrix = hCollisionConstraintArrayElement.child(self.aCollisionConstraintGeoMatrix).asMatrix()

                        #check collisionType
                        collisionConstraintType = hCollisionConstraintArrayElement.child(self.aCollisionConstraintType).asInt()
                        #sphere
                        if(collisionConstraintType == 0): self.collisionSphere(data, collisionGeo, collisionGeoMatrix)
                        #rectangle
                        if(collisionConstraintType == 1): self.collisionRectangle(data, collisionGeo, collisionGeoMatrix)
                    
                    else: 
                        if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint Geo at index %s not connected' % (index))
                else: 
                    if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint Geo Matrix at index %s not connected' % (index))
            else: 
                if(self.verbose):OpenMaya.MGlobal.displayInfo('CollisionConstraint at index %s not active' % (index))


    
    def collisionSphere(self, data, collisionGeo, collisionGeoMatrix):
        '''Update vertexPositionList with sphere collision'''

        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)

        #get vecSphereUp,Down
        collisionGeoPointArray = OpenMaya.MPointArray()
        fsCollisionGeo = OpenMaya.MFnMesh(collisionGeo)
        fsCollisionGeo.getPoints(collisionGeoPointArray)
        vecSphereUp = OpenMaya.MVector(collisionGeoPointArray[collisionGeoPointArray.length()-1] * collisionGeoMatrix)
        vecSphereDown = OpenMaya.MVector(collisionGeoPointArray[collisionGeoPointArray.length()-2] * collisionGeoMatrix)

        #get vecSphereCenter and radiusSphere
        vecDownToUp = vecSphereUp - vecSphereDown
        vecSphereCenter = vecSphereDown + (vecDownToUp * 0.5)
        radiusSphere = (vecDownToUp * 0.5).length()

        #iterate each vertex in vertexPositionList and perfom collision projection
        for index in range(vertexPositionList.length()):

            #get vecCurrentPoint
            vecCurrentPoint = OpenMaya.MVector(vertexPositionList[index])
            #get vecSphereCenterToCurrentPoint
            vecSphereCenterToCurrentPoint = vecCurrentPoint - vecSphereCenter
            vecSphereCenterToCurrentPointLength = vecSphereCenterToCurrentPoint.length()            
            #check if distance < radius
            if(vecSphereCenterToCurrentPointLength < radiusSphere):
                
                #project vecCurrentPoint outward
                vecCurrentPoint = vecSphereCenter + (vecSphereCenterToCurrentPoint.normal() * radiusSphere)
                #set vertexPositionList
                vertexPositionList.set(OpenMaya.MPoint(vecCurrentPoint) ,index)

        
        #set vertexPositionList to datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)

        #TMP
        if(self.verbose):OpenMaya.MGlobal.displayInfo('Sphere collision performed')



    def collisionRectangle(self, data, collisionGeo, collisionGeoMatrix):
        '''Update vertexPositionList with rectangle collision'''

        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)
        
        #iterate each vertex in vertexPositionList and perfom collision projection
        for index in range(vertexPositionList.length()):
            x = vertexPositionList[index].x
            y = vertexPositionList[index].y
            z = vertexPositionList[index].z
            vertexPositionList.set(OpenMaya.MPoint(x, 0, z),index)

        #set vertexPositionList to datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)

        #TMP
        if(self.verbose):OpenMaya.MGlobal.displayInfo('Rectangle collision performed')
         



    def stickConstraint(self, data):
        '''Constraint to maintain length of all edges of inputGeo'''

        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)
        #get edgeVertexIndexList
        edgeVertexIndexList = self.getMpointArrayFromDatablock(data, self.aEdgeVertexIndexList)
        #get edgeRestlengthList
        edgeRestlengthList = self.getMpointArrayFromDatablock(data, self.aEdgeRestlengthList)

        
        #iterate edges
        for index in range(edgeVertexIndexList.length()):
            #restlength
            restlength = edgeRestlengthList[index].x
            #vertexIndices
            vertexIndex1 = int(edgeVertexIndexList[index].x)
            vertexIndex2 = int(edgeVertexIndexList[index].y)
            #current vertex vectors
            vec1 = OpenMaya.MVector(vertexPositionList[vertexIndex1])
            vec2 = OpenMaya.MVector(vertexPositionList[vertexIndex2])
            #compute new position vectors
            delta = vec2 - vec1
            deltalength = delta.length()
            difference = (deltalength - restlength) / deltalength
            vec1 += delta*0.5*difference
            vec2 -= delta*0.5*difference

            #set positions in vertexPositionList
            vertexPositionList.set(OpenMaya.MPoint(vec1),vertexIndex1)
            vertexPositionList.set(OpenMaya.MPoint(vec2),vertexIndex2)


        #set vertexPositionList to datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)


    
    def positionConstraint(self, data):
        '''Constraint to maintain position of vertices'''
        
        #get vertexPositionList
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)

        #handle to positionConstraint array attr
        haPositionConstraintArray = data.inputArrayValue(self.aPositionConstraint)

        #iterate all array elements
        for index in range(haPositionConstraintArray.elementCount()):
            haPositionConstraintArray.jumpToArrayElement(index)
            #get datahandle to active attr for current array element
            hPositionConstraintArrayElement = haPositionConstraintArray.inputValue()
            hPositionConstraintActive = hPositionConstraintArrayElement.child(self.aPositionConstraintActive)
            #if current positionConstraint is active set its position values at index in vertexPositionList
            if(hPositionConstraintActive.asInt()):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('PositionConstraint at index %s active' % (index))
                #get vertexIndex
                vertexIndex = hPositionConstraintArrayElement.child(self.aPositionConstraintVertexIndex).asInt()
                #check if vertexIndex in vertexPositionList
                if(vertexIndex >= 0 and vertexIndex < vertexPositionList.length()):
                    #get positionConstraintCoordinate
                    vecPositionConstraintCoordinate = hPositionConstraintArrayElement.child(self.aPositionConstraintCoordinate).asFloatVector()
                    #set in vertexPositionList
                    vertexPositionList.set(OpenMaya.MPoint(vecPositionConstraintCoordinate), vertexIndex)
                else:
                    if(self.verbose):OpenMaya.MGlobal.displayInfo('VertexIndex: %s not in vertexPositionList' % (vertexIndex))
            
        
        #set vertexPositionList to datablock
        self.setMpointArrayToDatablock(data, self.aVertexPositionList, vertexPositionList)




    
    def draw(self, data):
        '''Set the outputGeo attr'''
        
        #get Pos from datablock
        vertexPositionList = self.getMpointArrayFromDatablock(data, self.aVertexPositionList)


        #get transformMatrix from datablock
        transformMatrix = data.inputValue(self.aTransformMatrix).asMatrix()
        
        
        #move all points in vertexPositionList to ws
        for index in range(vertexPositionList.length()):
            vertexPositionList.set(vertexPositionList[index], index)
        
        
        #get oInputGeo
        oInputGeo = data.inputValue(self.aInputGeo).asMesh()
        #fsInputGeo
        fsInputGeo = OpenMaya.MFnMesh(oInputGeo)


        #Create MeshDataBlock
        mfnOutMeshData = OpenMaya.MFnMeshData()
        outMeshDataBlock = mfnOutMeshData.create()
        
        #Create mesh obj as child of outMeshDataBlock
        polyCountArray = OpenMaya.MIntArray()
        polyConnectArray = OpenMaya.MIntArray()
        fsInputGeo.getVertices(polyCountArray, polyConnectArray)
        	
        #Create new geo with transformed point array
        mfnNewMesh = OpenMaya.MFnMesh()
        projectedGeo = mfnNewMesh.create(vertexPositionList.length(), polyCountArray.length(), vertexPositionList, polyCountArray, polyConnectArray, outMeshDataBlock)
        #Set OutPut Mesh data
        hOutMesh = data.outputValue(self.aOutputGeo)
        hOutMesh.setMObject(outMeshDataBlock)


    
    def getMpointArrayFromDatablock(self, data, aMpointArray):
        '''Convenience method to return MPointArray from datablock attribute'''

        pointArray = OpenMaya.MPointArray()
        oMPointArray = OpenMaya.MObject()
        oMPointArray = data.inputValue(aMpointArray).data()
        fsMPointArrayData = OpenMaya.MFnPointArrayData(oMPointArray)

        return fsMPointArrayData.array()



    def setMpointArrayToDatablock(self, data, aMpointArray, pointArray):
        '''Convenience method to set MPointArray to datablock attribute'''

        #pointArray to MObject
        fsPointArrayData = OpenMaya.MFnPointArrayData()
        oPointArray = fsPointArrayData.create(pointArray)

        #set aMpointArray
        hAMpointArray = data.outputValue(aMpointArray)
        hAMpointArray.setMObject(oPointArray)
        
        
        
    def getRepetitions(self, data):
        '''Return repetitions value from aRepetitions'''
        
        return data.inputValue(self.aRepetitions).asInt()
    
    
    
    def getCollisionConstraintGroundplaneActive(self, data):
        '''Return repetitions value from aRepetitions'''
        
        return data.inputValue(self.aCollisionConstraintGroundplaneActive).asInt()  



#Initialization
#-------------------------------------------------------------------------------------


def createTwClothSolverIOPaperBlockout():
    '''Create and deliver a pointer to an instance of TwClothSolverIOPaperBlockout'''
    return OpenMayaMPx.asMPxPtr(TwClothSolverIOPaperBlockout())


def initializeTwClothSolverIOPaperBlockout():
    '''Initializes the TwClothSolverIOPaperBlockout class factory with attributes. Is called only once when the plugin is registered.'''
    

    #Functionsets
    nAttr = OpenMaya.MFnNumericAttribute()
    tAttr = OpenMaya.MFnTypedAttribute()
    mAttr = OpenMaya.MFnMatrixAttribute()
    eAttr = OpenMaya.MFnEnumAttribute()
    cAttr = OpenMaya.MFnCompoundAttribute()

    
    
    
    #aInputGeo
    TwClothSolverIOPaperBlockout.aInputGeo = tAttr.create('inputGeo', 'inputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(True)
    tAttr.setReadable(False)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aInputGeo)
    
    
    #aOutputGeo
    TwClothSolverIOPaperBlockout.aOutputGeo = tAttr.create('outputGeo', 'outputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(False)
    tAttr.setReadable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aOutputGeo)

    
    #aStartFrame
    TwClothSolverIOPaperBlockout.aStartFrame = nAttr.create('startFrame','startFrame', OpenMaya.MFnNumericData.kInt, 1)
    nAttr.setKeyable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aStartFrame)

    
    #aCurrentFrame
    TwClothSolverIOPaperBlockout.aCurrentFrame = nAttr.create('currentFrame','currentFrame', OpenMaya.MFnNumericData.kFloat, 1)
    nAttr.setKeyable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aCurrentFrame)


    #aGravity
    TwClothSolverIOPaperBlockout.aGravity = nAttr.createPoint('gravity','gravity')
    nAttr.setKeyable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aGravity)


    #aVertexPositionList
    TwClothSolverIOPaperBlockout.aVertexPositionList = tAttr.create('vertexPositionList','vertexPositionList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aVertexPositionList)

    
    #aVertexOldpositionList
    TwClothSolverIOPaperBlockout.aVertexOldpositionList = tAttr.create('vertexOldpositionList','vertexOldpositionList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aVertexOldpositionList)


    #aVertexInitialpositionList
    TwClothSolverIOPaperBlockout.aVertexInitialpositionList = tAttr.create('vertexInitialpositionList','vertexInitialpositionList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aVertexInitialpositionList)


    #aEdgeVertexIndexList
    TwClothSolverIOPaperBlockout.aEdgeVertexIndexList = tAttr.create('edgeVertexIndexList','edgeVertexIndexList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aEdgeVertexIndexList)


    #aEdgeRestlengthList
    TwClothSolverIOPaperBlockout.aEdgeRestlengthList = tAttr.create('edgeRestlengthList','edgeRestlengthList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aEdgeRestlengthList)


    #aVertexForceList
    TwClothSolverIOPaperBlockout.aVertexForceList = tAttr.create('vertexForceList','vertexForceList', OpenMaya.MFnData.kPointArray)
    tAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aVertexForceList)


    #aTransformMatrix
    TwClothSolverIOPaperBlockout.aTransformMatrix = mAttr.create('transformMatrix', 'transformMatrix')
    mAttr.setReadable(False)
    mAttr.setHidden(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aTransformMatrix)

    
    #aVerbose
    TwClothSolverIOPaperBlockout.aVerbose = eAttr.create('verbose', 'verbose')
    eAttr.addField('noVerbose', 0)
    eAttr.addField('Verbose', 1)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aVerbose)


    #aRepetitions
    TwClothSolverIOPaperBlockout.aRepetitions = nAttr.create('repetitions','repetitions', OpenMaya.MFnNumericData.kInt, 1)
    nAttr.setKeyable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aRepetitions)


    #aPositionConstraintActive
    TwClothSolverIOPaperBlockout.aPositionConstraintActive = eAttr.create('positionConstraintActive', 'positionConstraintActive')
    eAttr.addField('Inactive', 0)
    eAttr.addField('Active', 1)
    #aPositionConstraintIndex
    TwClothSolverIOPaperBlockout.aPositionConstraintVertexIndex = nAttr.create('positionConstraintVertexIndex','positionConstraintVertexIndex', OpenMaya.MFnNumericData.kInt, 0)
    nAttr.setKeyable(True)
    #aPositionConstraintCoordinate
    TwClothSolverIOPaperBlockout.aPositionConstraintCoordinate = nAttr.createPoint('positionConstraintCoordinate','positionConstraintCoordinate')
    nAttr.setKeyable(True)

    #aPositionConstraint
    TwClothSolverIOPaperBlockout.aPositionConstraint = cAttr.create('positionConstraint','positionConstraint')
    cAttr.setArray(True)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aPositionConstraintActive)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aPositionConstraintVertexIndex)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aPositionConstraintCoordinate)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aPositionConstraint)


    #aCollisionConstraintActive
    TwClothSolverIOPaperBlockout.aCollisionConstraintActive = eAttr.create('collisionConstraintActive', 'collisionConstraintActive')
    eAttr.addField('Inactive', 0)
    eAttr.addField('Active', 1)
    #aCollisionConstraintType
    TwClothSolverIOPaperBlockout.aCollisionConstraintType = eAttr.create('collisionConstraintType', 'collisionConstraintType')
    eAttr.addField('Sphere', 0)
    eAttr.addField('Rectangle', 1)
    #aCollisionConstraintGeoMatrix
    TwClothSolverIOPaperBlockout.aCollisionConstraintGeoMatrix = mAttr.create('collisionConstraintGeoMatrix', 'collisionConstraintGeoMatrix')
    mAttr.setReadable(False)
    #aCollisionConstraintGeo
    TwClothSolverIOPaperBlockout.aCollisionConstraintGeo = tAttr.create('collisionConstraintGeo', 'collisionConstraintGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(True)
    tAttr.setReadable(False)
    
    #aCollisionConstraint
    TwClothSolverIOPaperBlockout.aCollisionConstraint = cAttr.create('collisionConstraint','collisionConstraint')
    cAttr.setArray(True)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aCollisionConstraintActive)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aCollisionConstraintType)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aCollisionConstraintGeoMatrix)
    cAttr.addChild(TwClothSolverIOPaperBlockout.aCollisionConstraintGeo)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aCollisionConstraint)
    

    #aCollisionConstraintGroundplaneActive
    TwClothSolverIOPaperBlockout.aCollisionConstraintGroundplaneActive = nAttr.create('collisionConstraintGroundplaneActive', 'collisionConstraintGroundplaneActive', OpenMaya.MFnNumericData.kBoolean, 0)
    nAttr.setStorable(False)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aCollisionConstraintGroundplaneActive)
    
    #aCollisionConstraintGroundplaneHeight
    TwClothSolverIOPaperBlockout.aCollisionConstraintGroundplaneHeight = nAttr.create('collisionConstraintGroundplaneHeight','collisionConstraintGroundplaneHeight', OpenMaya.MFnNumericData.kFloat, 0)
    nAttr.setKeyable(True)
    TwClothSolverIOPaperBlockout.addAttribute(TwClothSolverIOPaperBlockout.aCollisionConstraintGroundplaneHeight)

    
    #attributeAffects
    TwClothSolverIOPaperBlockout.attributeAffects(TwClothSolverIOPaperBlockout.aCurrentFrame, TwClothSolverIOPaperBlockout.aOutputGeo)








#Plugin Registration
#-------------------------------------------------------------------------------------

kPluginNodeName = "TwClothSolverIOPaperBlockout"
kPluginNodeId = OpenMaya.MTypeId(0x5700B)


def initializePlugin(mobject):
    '''Register Plugin'''
    
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.registerNode( kPluginNodeName, kPluginNodeId, createTwClothSolverIOPaperBlockout, initializeTwClothSolverIOPaperBlockout, OpenMayaMPx.MPxNode.kDependNode)
    except:
        sys.stderr.write( "Failed to register node: %s" % kPluginNodeName )
        raise


def uninitializePlugin(mobject):
    '''Deregister Plugin'''
    
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.deregisterNode( kPluginNodeId )
    except:
        sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeName )
        raise