

#Test Fixture for twClothSolver

#variables
sourcePath = 'H:/scripts/c++/twClothSolver/x64/maya_2012_release'
sourceName = 'twClothSolver.mll'
destinationPath = 'H:/scripts/temp/mayaPluginsTest'

#procedure
import pymel.core as pm
import shutil

#force restart of maya and unload plugin
#----------------------------------
try:
    #load new scene without saving
    pm.mel.eval('file -f -new;')
    #if plugin is loaded unload it
    if(pm.pluginInfo( sourceName, query=True, loaded=True )):pm.unloadPlugin( sourceName )
except:
	print('Error unloading Plugin')


#copy plugin
#----------------------------------
try:
    shutil.copy2(sourcePath +'/' +sourceName, destinationPath)
except:
    print('Error copying files')

#reload plugin
#----------------------------------

try:
    pm.loadPlugin( sourceName );print('Successfully reloaded plugin')
except:
	print('Error Reloading Plugin')



#create testFixture
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


#create outputGeo
#----------------------------------
#outputGeo
outputGeoTrans = pm.polyPlane( sx=10, sy=10, w=10, h=10, ch = False)[0]
outputGeoShape = outputGeoTrans.getShape()
pm.select(cl = True)

#create cloth solver node
#----------------------------------
clothSolver = pm.createNode('verletClothSolver')
clothSolver.verbose.set(1)
clothSolver.repetitions.set(100)
pm.select(cl = True)

#set clothSolver gravity
#----------------------------------
gravityPerSec = -9.8
clothSolver.gravity.set(0,gravityPerSec,0)

#make connections
#----------------------------------
#time
timeNode = pm.PyNode('time1')
pm.select(cl = True)
timeNode.outTime >> clothSolver.currentFrame
#inputGeo
inputGeoShape.outMesh >> clothSolver.inputGeo
inputGeoShape.parentMatrix >> clothSolver.transformMatrix
#outputGeo
clothSolver.outputGeo >> outputGeoShape.inMesh


#create constraints
#----------------------------------

#vcsPositionConstraint1
pm.select("{0}.vtx[0]".format(inputGeoTrans.name()), r = True)
pm.mel.eval('vcsPositionConstraint;')
pm.select(cl = True)

#vcsPositionConstraint2
pm.select("{0}.vtx[110]".format(inputGeoTrans.name()), r = True)
pm.mel.eval('vcsPositionConstraint;')
pm.select(cl = True)



#adjust timeline
#----------------------------------
pm.playbackOptions(ast = 1, aet = 1000, min = 1, max = 1000)

#select solver
#----------------------------------
pm.select(cl = True)
pm.select(clothSolver, r = True)