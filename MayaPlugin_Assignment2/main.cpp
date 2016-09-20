#include "includes.h"
#include "Callbacks.h"



static MCallbackId _TimeCallbackID;
static MCallbackId _NodeCreatedID;
static MCallbackId _NodeNameChangedID;
static MCallbackId _TransformChangedID;

static MCallbackIdArray polyCallbackIds;

static MCallbackId _VertChangedID;
static MCallbackId _EdgeChangedID;
static MCallbackId _FaceChangedID;
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
	
	

	//plugin.registerCommand("HelloWorld", HelloWorld::creator);
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
		_VertChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject, wantIdChanges, 3, VertChanged, NULL, &result);
		//_EdgeChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject,)
		//_FaceChangedID = MPolyMessage::addPolyComponentIdChangedCallback(Nullobject,)
		//MEventMessage::addEventCallback("testFunction",testFunction,)

	// Print to show plugin command was registered.
	MGlobal::displayInfo("Plugin is loaded");
	std::cout << "In initializePlugin()\n" << std::endl;
	return MS::kSuccess;
}

// Gets called when the plugin is unloaded from Maya.
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	//plugin.deregisterCommand("HelloWorld");

	// Print to show the plugin was unloaded.
	std::cout << "In uninitializePlugin()\n" << std::endl;

	MGlobal::displayInfo("Plugin is unloaded");

	MTimerMessage::removeCallback(_TimeCallbackID);
	MDGMessage	 ::removeCallback(_NodeCreatedID);
	MNodeMessage ::removeCallback(_NodeNameChangedID);
	MPolyMessage ::removeCallback(_VertChangedID);

	return MS::kSuccess;
}