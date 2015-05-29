


#twClothSolverIOPaperBlockout Test Fixture
#-------------------------------------------------------------------------------------


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




#TwClothSolverIOPaperBlockout
#----------------------------------
#----------------------------------

#create inputGeo
#----------------------------------
#inputGeo
inputGeoTrans = pm.polyPlane( sx=10, sy=10, w=10, h=10, ch = False)[0]
inputGeoShape = inputGeoTrans.getShape()
#transformPlane
inputGeoTrans.translateY.set(5)
inputGeoTrans.visibility.set(0)
pm.select(cl = True)
#pm.select(inputGeoTrans, r = True)
#pm.polyTriangulate(ch = False)
#pm.select(cl = True)

#create outputGeo
#----------------------------------
#outputGeo
outputGeoTrans = pm.polyPlane( sx=10, sy=10, w=10, h=10, ch = False)[0]
outputGeoShape = outputGeoTrans.getShape()
pm.select(cl = True)

#create cloth solver node
#----------------------------------
clothSolver = pm.createNode('TwClothSolverIOPaperBlockout')
clothSolver.verbose.set(0)
clothSolver.repetitions.set(10)
pm.select(cl = True)

#create positionConstraintLocators
#----------------------------------
positionConstraintLocatorIndex0Trans = pm.spaceLocator(n = 'positionConstraintLocatorIndex0')
positionConstraintLocatorIndex0Shape = positionConstraintLocatorIndex0Trans.getShape()
positionConstraintLocatorIndex0Trans.translate.set(-5,5,5)
pm.select(cl = True)

positionConstraintLocatorIndex110Trans = pm.spaceLocator(n = 'positionConstraintLocatorIndex110')
positionConstraintLocatorIndex110Shape = positionConstraintLocatorIndex110Trans.getShape()
positionConstraintLocatorIndex110Trans.translate.set(-5,5,-5)
pm.select(cl = True)

positionConstraintLocatorIndex120Trans = pm.spaceLocator(n = 'positionConstraintLocatorIndex120')
positionConstraintLocatorIndex120Shape = positionConstraintLocatorIndex120Trans.getShape()
positionConstraintLocatorIndex120Trans.translate.set(5,5,-5)
pm.select(cl = True)

positionConstraintLocatorIndex10Trans = pm.spaceLocator(n = 'positionConstraintLocatorIndex10')
positionConstraintLocatorIndex10Shape = positionConstraintLocatorIndex10Trans.getShape()
positionConstraintLocatorIndex10Trans.translate.set(5,5,5)
pm.select(cl = True)

positionConstraintLocatorIndex60Trans = pm.spaceLocator(n = 'positionConstraintLocatorIndex60')
positionConstraintLocatorIndex60Shape = positionConstraintLocatorIndex60Trans.getShape()
positionConstraintLocatorIndex60Trans.translate.set(0,5,0)
pm.select(cl = True)

#connect locators and set index attrs on clothSolver
#----------------------------------
clothSolver.positionConstraint[0].positionConstraintVertexIndex.set(0)
positionConstraintLocatorIndex0Shape.worldPosition >> clothSolver.positionConstraint[0].positionConstraintCoordinate
pm.select(cl = True)
clothSolver.positionConstraint[1].positionConstraintVertexIndex.set(110)
positionConstraintLocatorIndex110Shape.worldPosition >> clothSolver.positionConstraint[1].positionConstraintCoordinate
pm.select(cl = True)
clothSolver.positionConstraint[2].positionConstraintVertexIndex.set(120)
positionConstraintLocatorIndex120Shape.worldPosition >> clothSolver.positionConstraint[2].positionConstraintCoordinate
pm.select(cl = True)
clothSolver.positionConstraint[3].positionConstraintVertexIndex.set(10)
positionConstraintLocatorIndex10Shape.worldPosition >> clothSolver.positionConstraint[3].positionConstraintCoordinate
pm.select(cl = True)
clothSolver.positionConstraint[4].positionConstraintVertexIndex.set(60)
positionConstraintLocatorIndex60Shape.worldPosition >> clothSolver.positionConstraint[4].positionConstraintCoordinate
pm.select(cl = True)

#set clothSolver active attr
#----------------------------------
clothSolver.positionConstraint[0].positionConstraintActive.set(1)
clothSolver.positionConstraint[1].positionConstraintActive.set(1)

#create sphere
#----------------------------------
collisionSphereTrans = pm.polySphere(sx = 8,sy = 8, n = 'collisionSphere0', r = 2, ch = 0)[0]
collisionSphereShape = collisionSphereTrans.getShape()
collisionSphereTrans.translate.set(0,1,0)
pm.select(cl = True)

#connect sphere as collision object
#----------------------------------
clothSolver.collisionConstraint[0].collisionConstraintActive.set(1)
clothSolver.collisionConstraint[0].collisionConstraintType.set(0)
collisionSphereShape.outMesh >> clothSolver.collisionConstraint[0].collisionConstraintGeo
collisionSphereShape.parentMatrix >> clothSolver.collisionConstraint[0].collisionConstraintGeoMatrix
pm.select(cl = True)

#create cube
#----------------------------------
collisionCubeTrans = pm.polyCube(sx = 1,sy = 1, sz = 1, d = 4, w = 4, h = 4, n = 'collisionCube1', ch = 0)[0]
collisionCubeShape = collisionCubeTrans.getShape()
collisionCubeTrans.translate.set(5,0,0)
pm.select(cl = True)

#connect cube as collision object
#----------------------------------
clothSolver.collisionConstraint[1].collisionConstraintActive.set(1)
clothSolver.collisionConstraint[1].collisionConstraintType.set(1)
collisionCubeShape.outMesh >> clothSolver.collisionConstraint[1].collisionConstraintGeo
collisionCubeShape.parentMatrix >> clothSolver.collisionConstraint[1].collisionConstraintGeoMatrix
pm.select(cl = True)

#create pyramid
#----------------------------------
collisionPyramidTrans = pm.polyPyramid(ns = 5, w  = 4, n = 'collisionPyramid2', ch = 0)[0]
collisionPyramidShape = collisionPyramidTrans.getShape()
collisionPyramidTrans.translate.set(10,0,0)
pm.select(cl = True)

#connect Pyramid as collision object
#----------------------------------
clothSolver.collisionConstraint[2].collisionConstraintActive.set(1)
clothSolver.collisionConstraint[2].collisionConstraintType.set(1)
collisionPyramidShape.outMesh >> clothSolver.collisionConstraint[2].collisionConstraintGeo
collisionPyramidShape.parentMatrix >> clothSolver.collisionConstraint[2].collisionConstraintGeoMatrix
pm.select(cl = True)

#connect time
#----------------------------------
timeNode = pm.PyNode('time1')
pm.select(cl = True)
timeNode.outTime >> clothSolver.currentFrame

#connect inputGeo
#----------------------------------
inputGeoShape.outMesh >> clothSolver.inputGeo

#connect inputGeo parentMatrix
#----------------------------------
inputGeoShape.parentMatrix >> clothSolver.transformMatrix

#connect outputGeo
#----------------------------------
clothSolver.outputGeo >> outputGeoShape.inMesh

#set clothSolver gravity
#----------------------------------
gravityPerSec = -9.8
framesPerSec = 24
clothSolver.gravity.set(0,gravityPerSec/framesPerSec,0)

#set time range
#----------------------------------
pm.playbackOptions(ast = 1, aet = 1000, max = 1000, min = 1)

#rename and select clothSolver
#----------------------------------
clothSolverTrans = clothSolver.getParent()
pm.rename(clothSolver, 'twClothSolverShape')
pm.rename(clothSolverTrans, 'twClothSolver')
pm.select(clothSolver, r = True)