#include "includes.h"
#include "Callbacks.h"
#include "CallbackIds.h"



EXPORT MStatus initializePlugin(MObject obj) {
	std::cout.rdbuf(std::cerr.rdbuf());

	
	MStatus result = MS::kSuccess;

	// Set plugin registration info: Author, plugin-version and Maya version needed.
	MFnPlugin plugin(obj, "Martin_Clementson", "1.0", "2016", &result);

	if (MFAIL(result))
	{
		CHECK_MSTATUS(result);
		return result;
	}
	
	

	_TimeCallbackID     = MTimerMessage::addTimerCallback(5.0f, TimeCallback, NULL, &result);
	_NodeCreatedID		= MDGMessage::addNodeAddedCallback(NodeCreated,"dependNode",NULL,&result);
	//_TransformChangedID = MDagMessage::addWorldMatrixModifiedCallback()
	
	MObject Nullobject = MObject();
	if (!Nullobject.isNull())
		std::cerr << "Nullobject is not null" << std::endl;
	_NodeNameChangedID = MNodeMessage::addNameChangedCallback(Nullobject, NodeNameChanged, NULL, &result);
	
		// order is : kVertexIndex,kEdgeIndex,KFaceIndex
	
	bool wantIdChanges[3];
	wantIdChanges[MPolyMessage::kVertexIndex] = true;
	wantIdChanges[MPolyMessage::kEdgeIndex] = false;
	wantIdChanges[MPolyMessage::kFaceIndex] = false;
		//_VertChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject, wantIdChanges, 3, VertChanged, NULL, &result);
		//_EdgeChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject,)
		//_FaceChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject,)
		//MEventMessage::addEventCallback("testFunction",testFunction,)


			//std::cerr << thisTransform.fullPathName() << std::endl;

		MItDag transformIt(MItDag::kBreadthFirst, MFn::kTransform, &result);
		for (; !transformIt.isDone(); transformIt.next())
		{
		
			MFnTransform thisTransform(transformIt.currentItem());
			
			
			MDagPath path = MDagPath::getAPathTo(thisTransform.child(0));
					
			MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(path, transformNodeChanged, NULL, &result);
			if (result == MS::kSuccess)
			{
				if(transformCallbackIds.append(newId)== MS::kSuccess)
					std::cerr << "Transform callback added!  " << path.fullPathName() << std::endl;
				else
					std::cerr << "Could not add worldMatrix callback , Path: " << path.fullPathName() << std::endl;
				
			}
			else
				std::cerr << "Could not add worldMatrix callback , Path: "<< path.fullPathName() << std::endl;

		}


		MItDag meshIt(MItDag::kBreadthFirst, MFn::kMesh, &result);
		for (; !meshIt.isDone(); meshIt.next())
		{
					
			MFnMesh thisMesh(meshIt.currentItem());
			//MCallbackId polyId = MNodeMessage::addNodeDirtyCallback(meshIt.currentItem(), nodeIsDirty, NULL, &result);

			MCallbackId polyId =	MNodeMessage::addAttributeChangedCallback(meshIt.currentItem(), attrChangedCB,NULL,&result);
			
			//thisMesh.attribute("quadSplit",)
			
			if (result == MS::kSuccess)
			{
				polyCallbackIds.append(polyId);
				std::cerr << "Polychange callback added!  " << meshIt.currentItem().apiTypeStr() << std::endl;
			}
			else
				std::cerr << "error adding topologychange :" << meshIt.currentItem().apiTypeStr() << std::endl;


			 polyId = MPolyMessage::addPolyTopologyChangedCallback(meshIt.currentItem(), topologyChanged, NULL, &result);
			if (result == MS::kSuccess)
			{
				polyCallbackIds.append(polyId);
				std::cerr << "Polychange callback added!  " << meshIt.currentItem().apiTypeStr() << std::endl;
			}
		}
		

	// Print to show plugin command was registered.
	MGlobal::displayInfo("Plugin is loaded");
	std::cout << "In initializePlugin()\n" << std::endl;
	return MS::kSuccess;
}

// Gets called when the plugin is unloaded from Maya.
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	

	// Print to show the plugin was unloaded.
	std::cout << "In uninitializePlugin()\n" << std::endl;

	MGlobal::displayInfo("Plugin is unloaded");

	MTimerMessage::removeCallback(_TimeCallbackID);
	MDGMessage	 ::removeCallback(_NodeCreatedID);
	MNodeMessage ::removeCallback(_NodeNameChangedID);
	
	MMessage::removeCallbacks(polyCallbackIds);
	MMessage::removeCallbacks(transformCallbackIds);

	return MS::kSuccess;
}