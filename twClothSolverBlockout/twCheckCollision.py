


#twCheckCollision Module
#-------------------------------------------------------------------------------------


#Last Modified: 18.05.13
#Author: Timm Wagener
#Description: Simple check if points are inside a geometry



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
inputGeoTrans = pm.polyCube(ch = False)[0]
inputGeoShape = inputGeoTrans.getShape()
inputGeoTrans.translate.set(0,5,0)
pm.select(cl = True)
pm.rename(inputGeoTrans, 'inputGeo')

#create collisionGeo
#----------------------------------
#collisionGeo
collisionGeoTrans = pm.polyCube(ch = False)[0]
collisionGeoShape = collisionGeoTrans.getShape()
collisionGeoTrans.translate.set(0,2.5,0)
pm.select(cl = True)
pm.rename(collisionGeoTrans, 'collisionGeo')

#create TwCheckCollision
#----------------------------------
checkCollisionShape = pm.createNode('TwCheckCollision')
checkCollisionShape.verbose.set(1)
pm.select(cl = True)

#connect TwCheckCollision
#----------------------------------
inputGeoShape.outMesh >> checkCollisionShape.inputGeo
inputGeoShape.parentMatrix >> checkCollisionShape.inputGeoParentMatrix
collisionGeoShape.outMesh >> checkCollisionShape.collisionGeo
collisionGeoShape.parentMatrix >> checkCollisionShape.collisionGeoParentMatrix
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
checkCollisionShape.outputGeo >> outputGeoShape.inMesh
pm.select(cl = True)

#select inputGeo
#----------------------------------
pm.select(inputGeoTrans, r = True)
'''



#Imports
#-------------------------------------------------------------------------------------
import sys, math, time, random
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim
import maya.OpenMayaRender as OpenMayaRender
import maya.OpenMayaUI as OpenMayaUI



#TwCheckCollision class
#-------------------------------------------------------------------------------------

class TwCheckCollision(OpenMayaMPx.MPxLocatorNode):
    '''TwCheckCollision utility class to perform check wether or not points are intersecting other objects'''


    
    #Class Variables
    #-------------------------------------------------------------------------------------
    
    kPluginNodeName = "TwCheckCollision"
    kPluginNodeId = OpenMaya.MTypeId(0x5800B)

    aVerbose = OpenMaya.MObject()

    aInputGeo = OpenMaya.MObject()
    aInputGeoParentMatrix = OpenMaya.MObject()
    
    aCollisionGeo = OpenMaya.MObject()
    aCollisionGeoParentMatrix = OpenMaya.MObject()

    aOutputGeo = OpenMaya.MObject()
    
    

    
    
    #Methods
    #-------------------------------------------------------------------------------------
    
    def __init__(self):
        '''TwCheckCollision __init__'''

        #instance variables
        self.verbose = False
        self.inputGeoVertexCollisionList = None
        self.inputGeoVertexPositionList = OpenMaya.MPointArray()

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
        
        collisionGeo = data.inputValue(self.aCollisionGeo).asMesh()
        collisionGeoParentMatrix = data.inputValue(self.aCollisionGeoParentMatrix).asMatrix()




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
            #collisionGeo
            if not(self.attrConnected('collisionGeo')):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('collisionGeo not connected')
                return OpenMaya.MStatus.kSuccess
            #collisionGeoParentMatrix
            if not(self.attrConnected('collisionGeoParentMatrix')):
                if(self.verbose):OpenMaya.MGlobal.displayInfo('collisionGeoParentMatrix not connected')
                return OpenMaya.MStatus.kSuccess


            #3-Compute collision
            self.computeCollision(data)
            
            
            
        #set plug clean
        data.setClean(plug)

        if(self.verbose):OpenMaya.MGlobal.displayInfo('Compute executed')
        return OpenMaya.MStatus.kSuccess

            

    def draw(self, view, dagPath, displayStyle, displayStatus):
        '''Draws openGl to the viewport'''
        
        #check if self.inputGeoVertexCollisionList is true
        if not(self.inputGeoVertexCollisionList): return OpenMaya.MStatus.kSuccess

        #iterate vertexPositionList
        for index in range(self.inputGeoVertexPositionList.length()):

            view.beginGL()
            self.glFT.glPushAttrib( OpenMayaRender.MGL_ALL_ATTRIB_BITS )
        
            self.glFT.glPointSize(5.0)
            self.glFT.glEnable(OpenMayaRender.MGL_POINT_SMOOTH)

            self.glFT.glBegin(OpenMayaRender.MGL_POINTS)
        
            #Set color
            self.glFT.glColor3f( 0.0, 1.0, 0.0 )
            if(self.inputGeoVertexCollisionList[index]):self.glFT.glColor3f( 1.0, 0.0, 0.0 )
            #draw vertices
            x = self.inputGeoVertexPositionList[index].x
            y = self.inputGeoVertexPositionList[index].y
            z = self.inputGeoVertexPositionList[index].z
            self.glFT.glVertex3f(x, y, z)
        

            self.glFT.glEnd()
        
            self.glFT.glPopAttrib()
            view.endGL()
        
        



    
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

    
    
    def computeCollision(self, data):
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


        #get collisionGeoVertexPositionList
        collisionGeoVertexPositionList = OpenMaya.MPointArray()
        oCollisionGeo = data.inputValue(self.aCollisionGeo).asMesh()
        fsCollisionGeo = OpenMaya.MFnMesh(oCollisionGeo)
        fsCollisionGeo.getPoints(collisionGeoVertexPositionList)
        #get collisionGeoParentMatrix
        collisionGeoParentMatrix = data.inputValue(self.aCollisionGeoParentMatrix).asMatrix()
        #move inputGeoVertexPositionList to ws
        for index in range(collisionGeoVertexPositionList.length()):
            collisionGeoVertexPositionList.set(collisionGeoVertexPositionList[index] * collisionGeoParentMatrix ,index)


        #get collisionGeoTriangleVertexIndexList
        collisionGeoTriangleCountList = OpenMaya.MIntArray()
        collisionGeoTriangleVertexIndexList = OpenMaya.MIntArray()
        fsCollisionGeo.getTriangles(collisionGeoTriangleCountList, collisionGeoTriangleVertexIndexList)

        #get triangleCount
        triangleCount = 0
        for index in range(0,collisionGeoTriangleVertexIndexList.length(),3):
            triangleCount+=1


        #if verbose print collision geo triangle stats
        if(self.verbose):
            OpenMaya.MGlobal.displayInfo('CollisionGeo triangle stats')
            for index in range(0,collisionGeoTriangleVertexIndexList.length(),3):
                OpenMaya.MGlobal.displayInfo('TriangleID: %s VertexIndices: %s-%s-%s' % (index/3, collisionGeoTriangleVertexIndexList[index], collisionGeoTriangleVertexIndexList[index+1], collisionGeoTriangleVertexIndexList[index+2]))
        
        
        #inputGeoVertexCollisionList
        self.inputGeoVertexCollisionList = []

        #iterate and detect collision
        for index in range(inputGeoVertexPositionList.length()):
            collided = False
            collisionCounter = 0
            vecInputGeoPoint = OpenMaya.MVector(inputGeoVertexPositionList[index])
            for triangleIndex in range(0,collisionGeoTriangleVertexIndexList.length(),3):
                #get vecTrianglePointA,B,C
                vecTrianglePointA = OpenMaya.MVector(collisionGeoVertexPositionList[collisionGeoTriangleVertexIndexList[triangleIndex]])
                vecTrianglePointB = OpenMaya.MVector(collisionGeoVertexPositionList[collisionGeoTriangleVertexIndexList[triangleIndex+1]])
                vecTrianglePointC = OpenMaya.MVector(collisionGeoVertexPositionList[collisionGeoTriangleVertexIndexList[triangleIndex+2]])

                #get vecTriangleNormal
                vecTriangleNormal = ((vecTrianglePointB - vecTrianglePointA)^(vecTrianglePointC - vecTrianglePointA)).normal()

                #get planeDistance
                planeDistance = -1 * (vecTrianglePointA * vecTriangleNormal)

                #solve for vecInputGeoPoint
                collisionDecisionValue = (vecTriangleNormal*vecInputGeoPoint) + planeDistance
                #set collided var
                if(collisionDecisionValue >= 0): break
                else: collisionCounter += 1

            #Append collided value to inputGeoVertexCollisionList
            if(collisionCounter == triangleCount): collided = True
            self.inputGeoVertexCollisionList.append(collided)
        
        #set instance var for draw method
        self.inputGeoVertexPositionList = OpenMaya.MPointArray(inputGeoVertexPositionList)
    



#Initialization
#-------------------------------------------------------------------------------------


def createTwCheckCollision():
    '''Create and deliver a pointer to an instance of TwCheckCollision'''
    return OpenMayaMPx.asMPxPtr(TwCheckCollision())


def initializeTwCheckCollision():
    '''Initializes the TwCheckCollision class factory with attributes. Is called only once when the plugin is registered.'''
    

    #Functionsets
    tAttr = OpenMaya.MFnTypedAttribute()
    eAttr = OpenMaya.MFnEnumAttribute()
    mAttr = OpenMaya.MFnMatrixAttribute()
    


    #aVerbose
    TwCheckCollision.aVerbose = eAttr.create('verbose', 'verbose')
    eAttr.addField('noVerbose', 0)
    eAttr.addField('Verbose', 1)
    TwCheckCollision.addAttribute(TwCheckCollision.aVerbose)

    
    #aInputGeo
    TwCheckCollision.aInputGeo = tAttr.create('inputGeo', 'inputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(True)
    tAttr.setReadable(False)
    TwCheckCollision.addAttribute(TwCheckCollision.aInputGeo)
    #aInputGeoParentMatrix
    TwCheckCollision.aInputGeoParentMatrix = mAttr.create('inputGeoParentMatrix', 'inputGeoParentMatrix')
    mAttr.setReadable(False)
    mAttr.setHidden(True)
    TwCheckCollision.addAttribute(TwCheckCollision.aInputGeoParentMatrix)

    
    
    #aCollisionGeo
    TwCheckCollision.aCollisionGeo = tAttr.create('collisionGeo', 'collisionGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(True)
    tAttr.setReadable(False)
    TwCheckCollision.addAttribute(TwCheckCollision.aCollisionGeo)
    #aCollisionGeoParentMatrix
    TwCheckCollision.aCollisionGeoParentMatrix = mAttr.create('collisionGeoParentMatrix', 'collisionGeoParentMatrix')
    mAttr.setReadable(False)
    mAttr.setHidden(True)
    TwCheckCollision.addAttribute(TwCheckCollision.aCollisionGeoParentMatrix)

    
    
    #aOutputGeo
    TwCheckCollision.aOutputGeo = tAttr.create('outputGeo', 'outputGeo', OpenMaya.MFnData.kMesh)
    tAttr.setWritable(False)
    tAttr.setReadable(True)
    TwCheckCollision.addAttribute(TwCheckCollision.aOutputGeo)
    
    
    
    #attributeAffects
    TwCheckCollision.attributeAffects(TwCheckCollision.aInputGeo, TwCheckCollision.aOutputGeo)
    TwCheckCollision.attributeAffects(TwCheckCollision.aInputGeoParentMatrix, TwCheckCollision.aOutputGeo)

    TwCheckCollision.attributeAffects(TwCheckCollision.aCollisionGeo, TwCheckCollision.aOutputGeo)
    TwCheckCollision.attributeAffects(TwCheckCollision.aCollisionGeoParentMatrix, TwCheckCollision.aOutputGeo)