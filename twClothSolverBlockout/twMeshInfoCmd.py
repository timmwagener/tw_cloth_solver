


#twMeshInfoCmd Module
#-------------------------------------------------------------------------------------




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


#MeshInfo
#----------------------------------
#----------------------------------
pm.mel.eval('MeshInfo')

'''





#Imports
#-------------------------------------------------------------------------------------
import sys, math, time, random
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim







#TwMeshInfoCmd class
#-------------------------------------------------------------------------------------
class TwMeshInfoCmd(OpenMayaMPx.MPxCommand):
    '''MeshInfo utility cmd to display mesh information'''

    def __init__(self):
        '''Constructor'''

        #Instance variables
        self.verbose = True
        self.dividerString = '-------------------------------'

        OpenMayaMPx.MPxCommand.__init__(self)


    def doIt(self, args):
        '''Function that is executed when cmd is executed'''

        #get selectionList
        selectionList = OpenMaya.MSelectionList()
        OpenMaya.MGlobal.getActiveSelectionList( selectionList )

        #return if nothing is selected
        if(selectionList.length() == 0): 
            if(self.verbose): OpenMaya.MGlobal.displayInfo('Nothing selected. Please select a poly object.')
            return OpenMaya.MStatus.kSuccess


        #iterate objects, if == polymesh display infos
        itSelectionList = OpenMaya.MItSelectionList(selectionList)

        #iterate
        while not(itSelectionList.isDone()):
            
            #get current object and create functionset
            currentObject = OpenMaya.MObject()
            itSelectionList.getDependNode(currentObject)
            currentObjectDagPath = OpenMaya.MDagPath()
            itSelectionList.getDagPath(currentObjectDagPath)
            fsDepNodeCurrentObject = OpenMaya.MFnDependencyNode(currentObject)

            
            #check if current object poly mesh if not continue
            if not(fsDepNodeCurrentObject.typeName() == 'mesh'):
                if(self.verbose): OpenMaya.MGlobal.displayInfo('%s is not a mesh. Its of type: %s' %(fsDepNodeCurrentObject.name(), fsDepNodeCurrentObject.typeName()))
                itSelectionList.next()
                continue

            #else display name
            OpenMaya.MGlobal.displayInfo('%s' % (fsDepNodeCurrentObject.name()))
            OpenMaya.MGlobal.displayInfo(self.dividerString)

            #create mesh functionset
            fsMeshCurrentObject = OpenMaya.MFnMesh(currentObjectDagPath)
            #create edge iterator
            itMeshEdges = OpenMaya.MItMeshEdge(currentObject)
            #iterate edges and display infos
            while not(itMeshEdges.isDone()):
                
                edgeIndex = itMeshEdges.index()

                #print edgeIndex
                OpenMaya.MGlobal.displayInfo('Edge Index %s' % (edgeIndex))
                
                #print Vertex IDs for edgeIndex
                util = OpenMaya.MScriptUtil()
                util.createFromDouble(0.0)
                ptr = util.asInt2Ptr()
                fsMeshCurrentObject.getEdgeVertices(edgeIndex, ptr)
                
                vertexIndex1 = util.getInt2ArrayItem(ptr, 0,0)
                vertexIndex2 = util.getInt2ArrayItem(ptr, 0,1)
                OpenMaya.MGlobal.displayInfo('VertexIds: %s %s' %(vertexIndex1, vertexIndex2))

                #print worldSpace position from vertex IDs of edge index
                pointIndex1 = OpenMaya.MPoint()
                pointIndex2 = OpenMaya.MPoint()
                fsMeshCurrentObject.getPoint(vertexIndex1, pointIndex1, OpenMaya.MSpace.kWorld)
                fsMeshCurrentObject.getPoint(vertexIndex2, pointIndex2, OpenMaya.MSpace.kWorld)

                OpenMaya.MGlobal.displayInfo('VertexId: %s - %.3f,%.3f,%.3f' %(vertexIndex1, pointIndex1.x, pointIndex1.y, pointIndex1.z))
                OpenMaya.MGlobal.displayInfo('VertexId: %s - %.3f,%.3f,%.3f' %(vertexIndex2, pointIndex2.x, pointIndex2.y, pointIndex2.z))

                itMeshEdges.next()

            itSelectionList.next()

        



        #Success
        if(self.verbose): OpenMaya.MGlobal.displayInfo('MeshInfo cmd executed successfully')
        return OpenMaya.MStatus.kSuccess






#Initialization
#-------------------------------------------------------------------------------------

def createTwMeshInfoCmd():
    '''Creator function for TwMeshInfoCmd. Delivers pointer to instance'''
    return OpenMayaMPx.asMPxPtr(TwMeshInfoCmd())