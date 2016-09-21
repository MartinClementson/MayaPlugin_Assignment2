#pragma once
#include "includes.h"

void TimeCallback(float elapsedTime, float lastTime, void *clientData)
{
	static float time = 0;
	time += elapsedTime;
	//std::cerr << "5 Second Callback is triggered" << "\n";
	//std::cerr << "Elapsed time : " << ceil(time) << "\n";

}

void NodeCreated(MObject &node, void *clientData)
{
	if (node.hasFn(MFn::kDagNode))
	{
		MFnDagNode nodeHandle(node);
		
			
			std::cerr << "A new node was created \n";
			std::cerr << "Callback triggered from  node : " << nodeHandle.name() << "\n"
			<< "Node Path : " << nodeHandle.fullPathName() << "\n" << std::endl;
		
	}
}

void NodeNameChanged(MObject &node, const MString & str,void *clientData)
{
	if (node.hasFn(MFn::kMesh)) //MFn::kDagNode  is default
	{
		MFnDagNode nodeHandle(node);
		
			std::cerr << "A node has changed name \n"
			<< "Node: " << str << "\n"
			<< "New name: " << nodeHandle.name() << "\n"
			<< "Node Path : " << nodeHandle.fullPathName() << "\n" << std::endl;
		
	}


		

}




void nodeIsDirty(MObject &node, void *clientData)
{
	MFnMesh obj(node);
	std::cerr << "A Mesh has changed has changed!! |" << obj.name() << std::endl;
}


void transformNodeChanged(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	MFnTransform obj(transformNode);
	std::cerr << "A TransformNode has changed!! |" << obj.name() << std::endl;

 }


void userCB(MNodeMessage::AttributeMessage msg, MPlug & plug,
	MPlug & otherPlug, void*)
{
	
	
	std::cerr << "A Mesh has changed!! |" << plug.info() << std::endl;
	std::cerr << "Attribute : " << plug.className() << std::endl;


	//plug.attribute().apiTypeStr()
}