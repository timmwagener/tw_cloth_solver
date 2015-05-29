

#Import
#-------------------------------------------------------------------------------------
import maya.OpenMayaMPx as OpenMayaMPx
#PlugIns
import twClothSolverIOPaperBlockout
import twMeshInfoCmd
import twCheckCollision
import twDrawBB


#reload
doReload = True
if(doReload): reload(twClothSolverIOPaperBlockout),reload(twMeshInfoCmd), reload(twCheckCollision), reload(twDrawBB)






#Plugin Registration
#-------------------------------------------------------------------------------------


def initializePlugin(mobject):
    '''Register Plugin'''
    
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.registerCommand('MeshInfo', twMeshInfoCmd.createTwMeshInfoCmd)
        mplugin.registerNode( twCheckCollision.TwCheckCollision.kPluginNodeName, twCheckCollision.TwCheckCollision.kPluginNodeId, twCheckCollision.createTwCheckCollision, twCheckCollision.initializeTwCheckCollision, OpenMayaMPx.MPxNode.kLocatorNode)
        mplugin.registerNode( twDrawBB.TwDrawBB.kPluginNodeName, twDrawBB.TwDrawBB.kPluginNodeId, twDrawBB.createTwDrawBB, twDrawBB.initializeTwDrawBB, OpenMayaMPx.MPxNode.kLocatorNode)
        mplugin.registerNode( twClothSolverIOPaperBlockout.TwClothSolverIOPaperBlockout.kPluginNodeName, twClothSolverIOPaperBlockout.TwClothSolverIOPaperBlockout.kPluginNodeId, twClothSolverIOPaperBlockout.createTwClothSolverIOPaperBlockout, twClothSolverIOPaperBlockout.initializeTwClothSolverIOPaperBlockout, OpenMayaMPx.MPxNode.kLocatorNode)
    except:
        sys.stderr.write( "Failed to register node: %s" % kPluginNodeName )
        raise


def uninitializePlugin(mobject):
    '''Deregister Plugin'''
    
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.deregisterCommand('MeshInfo')
        mplugin.deregisterNode( twCheckCollision.TwCheckCollision.kPluginNodeId )
        mplugin.deregisterNode( twDrawBB.TwDrawBB.kPluginNodeId )
        mplugin.deregisterNode( twClothSolverIOPaperBlockout.TwClothSolverIOPaperBlockout.kPluginNodeId )
    except:
        sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeName )
        raise