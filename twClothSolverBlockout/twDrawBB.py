


#twDrawBB Module
#-------------------------------------------------------------------------------------


#Last Modified: 18.05.13
#Author: Timm Wagener
#Description: Simple drawing of BBox



#Setup Script
#-------------------------------------------------------------------------------------

'''
import pymel.core as pm


#reload plugin
#----------------------------------
try:
    #load new scene without saving
    pm.mel.eval('file -f -new;')
    #if plugin is loaded unload and reload it, else reload
    if(pm.pluginInfo( 'twClothSolverBlockoutPlugins.py', query=True, loaded=True )):
        pm.unloadPlugin( 'twClothSolverBlockoutPlugins.py' )
        pm.loadPlugin( 'twClothSolverBlockoutPlugins.py' )
        print('Successfully reloaded plugin')
    else:
        pm.loadPlugin( 'twClothSolverBlockoutPlugins.py' )
        print('Successfully loaded plugin')
except:
	print('Error Reloading Plugin')


#create inputGeo
#----------------------------------
#inputGeo
inputGeoTrans = pm.polyHelix(c = 5, h = 12, w = 10, r = 1, cuv = 3, ch = False, n = 'BBCheckHelix')[0]
inputGeoShape = inputGeoTrans.getShape()
inputGeoTrans.translate.set(0,5,0)
pm.select(cl = True)

#create TwDrawBB
#----------------------------------
drawBBShape = pm.createNode('TwDrawBB')
drawBBShape.verbose.set(1)
drawBBShape.bBoxType.set(1)
pm.select(cl = True)

#connect TwCheckCollision
#----------------------------------
inputGeoShape.outMesh >> drawBBShape.inputGeo
inputGeoShape.parentMatrix >> drawBBShape.inputGeoParentMatrix
pm.select(cl = True)

#create outputGeo
#----------------------------------
#outputGeo
outputGeoTrans = pm.polyCube(ch = False)[0]
outputGeoShape = outputGeoTrans.getShape()
pm.select(cl = True)
pm.rename(outputGeoTrans, 'outputGeo')

#connect outputGeo
#----------------------------------
drawBBShape.outputGeo >> outputGeoShape.inMesh
pm.select(cl = True)

#select inputGeo
#----------------------------------
pm.select(drawBBShape, r = True)
'''



#Imports
#-------------------------------------------------------------------------------------
import sys, math, time, random
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim
import maya.OpenMayaRender as OpenMayaRender
import maya.OpenMayaUI as OpenMayaUI



#TwDrawBB class
#-------------------------------------------------------------------------------------

class TwDrawBB(OpenMayaMPx.MPxLocatorNode):
    '''TwDrawBB utility class to calculate, test and draw different BBox methods'''


    
    #Class Variables
    #-------------------------------------------------------------------------------------
    
    kPluginNodeName = "TwDrawBB"
    kPluginNodeId = OpenMaya.MTypeId(0x5600B)

    aVerbose = OpenMaya.MObject()

    aInputGeo = OpenMaya.MObject()
    aInputGeoParentMatrix = OpenMaya.MObject()

    aBBoxType = OpenMaya.MObject()
    
    aOutputGeo = OpenMaya.MObject()
    
    

    
    
    #Methods
    #-------------------------------------------------------------------------------------
    
    def __init__(self):
        '''TwDrawBB __init__'''

        #instance variables
        self.verbose = False
        self.minBBoxPoint = []
        self.maxBBoxPoint = []
        self.bBoxType = 0
        self.inputGeoParentMatrix = None

        #aGlRenderer will be initialized with Hardware renderer
        self.glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
        
        #aGlFT hold reference to OpenGl fucntion table used by maya
        self.glFT = self.glRenderer.glFunctionTable()
        
        #Execute superclass constructor
        OpenMayaMPx.MPxLocatorNode.__init__(self)

        

    def compute(self, plug, data):
        '''Compute'''
        
        #Get all attrs to force update
        inputGeo = data.inputValue(self.aInputGeo).asMesh()
        inputGeoParentMatrix = data.inputValue(self.aInputGeoParentMatrix).asMatrix()
        
        
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
            #inputGeoParentMatrix
            if not(self.attrConnected('inputGeoParentMatrix')):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('inputGeoParentMatrix not connected')
                return OpenMaya.MStatus.kSuccess
            
            
            #3-Compute BBox
            self.computeBBox(data)
            
            
            
        #set plug clean
        data.setClean(plug)

        if(self.verbose):OpenMaya.MGlobal.displayInfo('Compute executed')
        return OpenMaya.MStatus.kSuccess

            

    def draw(self, view, dagPath, displayStyle, displayStatus):
        '''Draws openGl to the viewport'''
        
        
        #check if self.minBBoxPoint is true
        if not(self.minBBoxPoint): return OpenMaya.MStatus.kSuccess
        
        #determine drawType
        if(self.bBoxType == 0): self.drawBBoxAABB(view, dagPath, displayStyle, displayStatus)
        elif(self.bBoxType == 1): self.drawBBoxOOB(view, dagPath, displayStyle, displayStatus)
        
        return OpenMaya.MStatus.kSuccess
        



    
    #Utility methods
    #-------------------------------------------------------------------------------------

    def setVerbose(self, data):
        '''Set instance variable verbose for output messages'''

        if(data.inputValue(self.aVerbose).asInt()): self.verbose = True
        else:self.verbose = False


    def getBBoxType(self, data):
        '''Convenience method to get BBoxType Attr value'''

        return data.inputValue(self.aBBoxType).asInt()

    
    
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

    
    
    def computeBBox(self, data):
        '''Compute collision for inputGeo and collisionGeo'''

        #get inputGeoVertexPositionList
        inputGeoVertexPositionList = OpenMaya.MPointArray()
        oInputGeo = data.inputValue(self.aInputGeo).asMesh()
        fsInputGeo = OpenMaya.MFnMesh(oInputGeo)
        fsInputGeo.getPoints(inputGeoVertexPositionList)
        #get inputGeoParentMatrix
        inputGeoParentMatrix = data.inputValue(self.aInputGeoParentMatrix).asMatrix()
        #move inputGeoVertexPositionList to ws
        for index in range(inputGeoVertexPositionList.length()):
            inputGeoVertexPositionList.set(inputGeoVertexPositionList[index] * inputGeoParentMatrix ,index)
        

        #execute BBox
        #AABB
        if not(self.getBBoxType(data)): self.bBoxType = 0; self.computeBBoxAABB(inputGeoVertexPositionList)
        #OOB
        elif(self.getBBoxType(data) == 1): self.bBoxType = 1; self.computeBBoxOOB(inputGeoVertexPositionList, inputGeoParentMatrix)



    def computeBBoxAABB(self, inputGeoVertexPositionList):
        '''Compute AxisAligned BBox'''
        
        #iterate inputGeoVertexPositionList to find minPoint and maxPoint
        minX = self.getMin('x', inputGeoVertexPositionList)
        minY = self.getMin('y', inputGeoVertexPositionList)
        minZ = self.getMin('z', inputGeoVertexPositionList)

        maxX = self.getMax('x', inputGeoVertexPositionList)
        maxY = self.getMax('y', inputGeoVertexPositionList)
        maxZ = self.getMax('z', inputGeoVertexPositionList)

        #set instance vars
        self.minBBoxPoint = [minX, minY, minZ]
        self.maxBBoxPoint = [maxX, maxY, maxZ]



    def computeBBoxOOB(self, inputGeoVertexPositionList, inputGeoParentMatrix):
        '''Compute OOB BBox'''

        #move inputGeoVertexPositionList to Local Space
        for index in range(inputGeoVertexPositionList.length()):
            inputGeoVertexPositionList.set(inputGeoVertexPositionList[index] * inputGeoParentMatrix.inverse() ,index)

        #iterate inputGeoVertexPositionList to find minPoint and maxPoint
        minX = self.getMin('x', inputGeoVertexPositionList)
        minY = self.getMin('y', inputGeoVertexPositionList)
        minZ = self.getMin('z', inputGeoVertexPositionList)

        maxX = self.getMax('x', inputGeoVertexPositionList)
        maxY = self.getMax('y', inputGeoVertexPositionList)
        maxZ = self.getMax('z', inputGeoVertexPositionList)

        #create mPoints and multiply with inputGeoParentMatrix
        minPoint = OpenMaya.MPoint(minX, minY, minZ)
        maxPoint = OpenMaya.MPoint(maxX, maxY, maxZ)

        #set instance vars
        self.minBBoxPoint = [minPoint.x, minPoint.y, minPoint.z]
        self.maxBBoxPoint = [maxPoint.x, maxPoint.y, maxPoint.z]

        self.inputGeoParentMatrix = inputGeoParentMatrix


    
    def getMin(self, type, pointArray):
        '''Get minimum value out of mPointArray'''

        minValue = None
        for index in range(pointArray.length()):
            if not(index):
                if(type == 'x'):minValue = pointArray[index].x
                elif(type == 'y'):minValue = pointArray[index].y
                elif(type == 'z'):minValue = pointArray[index].z

            else:
                if(type == 'x'):
                    if(minValue > pointArray[index].x): minValue = pointArray[index].x
                if(type == 'y'):
                    if(minValue > pointArray[index].y): minValue = pointArray[index].y
                if(type == 'z'):
                    if(minValue > pointArray[index].z): minValue = pointArray[index].z

        return minValue



    def getMax(self, type, pointArray):
        '''Get minimum value out of mPointArray'''

        maxValue = None
        for index in range(pointArray.length()):
            if not(index):
                if(type == 'x'):maxValue = pointArray[index].x
                elif(type == 'y'):maxValue = pointArray[index].y
                elif(type == 'z'):maxValue = pointArray[index].z

            else:
                if(type == 'x'):
                    if(maxValue < pointArray[index].x): maxValue = pointArray[index].x
                if(type == 'y'):
                    if(maxValue < pointArray[index].y): maxValue = pointArray[index].y
                if(type == 'z'):
                    if(maxValue < pointArray[index].z): maxValue = pointArray[index].z

        return maxValue



    def drawBBoxAABB(self, view, dagPath, displayStyle, displayStatus):
        '''Draw BBox for mode AABB'''

        #draw BBox from minBBoxPoint and maxBBoxPoint
        view.beginGL()
        self.glFT.glPushAttrib( OpenMayaRender.MGL_ALL_ATTRIB_BITS )
        

        #Draw Vertices
        #-------------------------------------------------------------------------------------
        self.glFT.glPointSize(5.0)
        self.glFT.glEnable(OpenMayaRender.MGL_POINT_SMOOTH)

        self.glFT.glBegin(OpenMayaRender.MGL_POINTS)
        
        #Set color
        self.glFT.glColor3f( 0.0, 1.0, 0.0 )

        #draw vertices
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])

        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])
        

        self.glFT.glEnd()
        #-------------------------------------------------------------------------------------


        #Draw Edges
        #-------------------------------------------------------------------------------------
        self.glFT.glBegin(OpenMayaRender.MGL_LINES)
        
        #Set color
        self.glFT.glColor3f( 0.0, 1.0, 0.0 )

        #draw vertices
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])

        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])

        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])


        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])

        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])

        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])


        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])

        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])


        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])

        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])


        self.glFT.glVertex3f(self.minBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2])
        self.glFT.glVertex3f(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2])

        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2])
        self.glFT.glVertex3f(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2])
        

        self.glFT.glEnd()
        #-------------------------------------------------------------------------------------
        
        self.glFT.glPopAttrib()
        view.endGL()


    def drawBBoxOOB(self, view, dagPath, displayStyle, displayStatus):
        '''Draw BBox for type OOB'''
        
        

        #create points from min and maxPoints and move them to ws
        pointA = OpenMaya.MPoint(self.minBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2]) * self.inputGeoParentMatrix
        pointB = OpenMaya.MPoint(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.minBBoxPoint[2]) * self.inputGeoParentMatrix
        pointC = OpenMaya.MPoint(self.minBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2]) * self.inputGeoParentMatrix
        pointD = OpenMaya.MPoint(self.maxBBoxPoint[0], self.minBBoxPoint[1], self.maxBBoxPoint[2]) * self.inputGeoParentMatrix

        pointE = OpenMaya.MPoint(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2]) * self.inputGeoParentMatrix
        pointF = OpenMaya.MPoint(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.maxBBoxPoint[2]) * self.inputGeoParentMatrix
        pointG = OpenMaya.MPoint(self.maxBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2]) * self.inputGeoParentMatrix
        pointH = OpenMaya.MPoint(self.minBBoxPoint[0], self.maxBBoxPoint[1], self.minBBoxPoint[2]) * self.inputGeoParentMatrix

        
        #OpenGL
        #-------------------------------------------------------------------------------------
        view.beginGL()
        self.glFT.glPushAttrib( OpenMayaRender.MGL_ALL_ATTRIB_BITS )
        

        #Draw Vertices
        #-------------------------------------------------------------------------------------
        self.glFT.glPointSize(5.0)
        self.glFT.glEnable(OpenMayaRender.MGL_POINT_SMOOTH)

        self.glFT.glBegin(OpenMayaRender.MGL_POINTS)
        
        #Set color
        self.glFT.glColor3f( 0.0, 1.0, 0.0 )

        #draw vertices
        self.glFT.glVertex3f(pointA.x, pointA.y, pointA.z)
        self.glFT.glVertex3f(pointB.x, pointB.y, pointB.z)
        self.glFT.glVertex3f(pointC.x, pointC.y, pointC.z)
        self.glFT.glVertex3f(pointD.x, pointD.y, pointD.z)

        self.glFT.glVertex3f(pointE.x, pointE.y, pointE.z)
        self.glFT.glVertex3f(pointF.x, pointF.y, pointF.z)
        self.glFT.glVertex3f(pointG.x, pointG.y, pointG.z)
        self.glFT.glVertex3f(pointH.x, pointH.y, pointH.z)
        

        self.glFT.glEnd()
        #-------------------------------------------------------------------------------------

        
        #Draw Edges
        #-------------------------------------------------------------------------------------
        self.glFT.glBegin(OpenMayaRender.MGL_LINES)
        
        #Set color
        self.glFT.glColor3f( 0.0, 1.0, 0.0 )

        #draw vertices
        self.glFT.glVertex3f(pointA.x, pointA.y, pointA.z)
        self.glFT.glVertex3f(pointB.x, pointB.y, pointB.z)

        self.glFT.glVertex3f(pointB.x, pointB.y, pointB.z)
        self.glFT.glVertex3f(pointD.x, pointD.y, pointD.z)

        self.glFT.glVertex3f(pointD.x, pointD.y, pointD.z)
        self.glFT.glVertex3f(pointC.x, pointC.y, pointC.z)

        self.glFT.glVertex3f(pointC.x, pointC.y, pointC.z)
        self.glFT.glVertex3f(pointA.x, pointA.y, pointA.z)


        self.glFT.glVertex3f(pointE.x, pointE.y, pointE.z)
        self.glFT.glVertex3f(pointF.x, pointF.y, pointF.z)

        self.glFT.glVertex3f(pointF.x, pointF.y, pointF.z)
        self.glFT.glVertex3f(pointH.x, pointH.y, pointH.z)

        self.glFT.glVertex3f(pointH.x, pointH.y, pointH.z)
        self.glFT.glVertex3f(pointG.x, pointG.y, pointG.z)

        self.glFT.glVertex3f(pointG.x, pointG.y, pointG.z)
        self.glFT.glVertex3f(pointE.x, pointE.y, pointE.z)


        self.glFT.glVertex3f(pointD.x, pointD.y, pointD.z)
        self.glFT.glVertex3f(pointE.x, pointE.y, pointE.z)

        self.glFT.glVertex3f(pointA.x, pointA.y, pointA.z)
        self.glFT.glVertex3f(pointH.x, pointH.y, pointH.z)

        self.glFT.glVertex3f(pointB.x, pointB.y, pointB.z)
        self.glFT.glVertex3f(pointG.x, pointG.y, pointG.z)

        self.glFT.glVertex3f(pointF.x, pointF.y, pointF.z)
        self.glFT.glVertex3f(pointC.x, pointC.y, pointC.z)
        

        self.glFT.glEnd()
        #-------------------------------------------------------------------------------------

        
        self.glFT.glPopAttrib()
        view.endGL()



#Initialization
#-------------------------------------------------------------------------------------


def createTwDrawBB():
    '''Create and deliver a pointer to an instance of TwDrawBB'''
    return OpenMayaMPx.asMPxPtr(TwDrawBB())


def initializeTwDrawBB():
    '''Initializes the TwDrawBB class factory with attributes. Is called only once when the plugin is registered.'''
    

    #Functionsets
    tAttr = OpenMaya.MFnTypedAttribute()
    eAttr = OpenMaya.MFnEnumAttribute()
    mAttr = OpenMaya.MFnMatrixAttribute()
    


    #aVerbose
    TwDrawBB.aVerbose = eAttr.create('verbose', 'verbose')
    eAttr.addField('noVerbose', 0)
    eAttr.addField('Verbose', 1)
    TwDrawBB.addAttribute(TwDrawBB.aVerbose)

    
    #aInputGeo
    TwDrawBB.aInputGeo = tAttr.create('inputGeo', 'inputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(True)
    tAttr.setReadable(False)
    TwDrawBB.addAttribute(TwDrawBB.aInputGeo)
    #aInputGeoParentMatrix
    TwDrawBB.aInputGeoParentMatrix = mAttr.create('inputGeoParentMatrix', 'inputGeoParentMatrix')
    mAttr.setReadable(False)
    mAttr.setHidden(True)
    TwDrawBB.addAttribute(TwDrawBB.aInputGeoParentMatrix)


    #aBBoxType
    TwDrawBB.aBBoxType = eAttr.create('bBoxType', 'bBoxType')
    eAttr.addField('AABB', 0)
    eAttr.addField('OOB', 1)
    TwDrawBB.addAttribute(TwDrawBB.aBBoxType)

    
    #aOutputGeo
    TwDrawBB.aOutputGeo = tAttr.create('outputGeo', 'outputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(False)
    tAttr.setReadable(True)
    TwDrawBB.addAttribute(TwDrawBB.aOutputGeo)
    
    
    
    #attributeAffects
    TwDrawBB.attributeAffects(TwDrawBB.aInputGeo, TwDrawBB.aOutputGeo)
    TwDrawBB.attributeAffects(TwDrawBB.aInputGeoParentMatrix, TwDrawBB.aOutputGeo)
    TwDrawBB.attributeAffects(TwDrawBB.aBBoxType, TwDrawBB.aOutputGeo)