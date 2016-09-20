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
	if (node.hasFn(MFn::kDagNode))
	{
		MFnDagNode nodeHandle(node);
		
		

			std::cerr << "A node has changed name \n"
			<< "Node: " << str << "\n"
			<< "New name: " << nodeHandle.name() << "\n"
			<< "Node Path : " << nodeHandle.dagPath().fullPathName() << "\n" << std::endl;
		
		
	}


		

}

void VertChanged(MUintArray componentIds[], unsigned int count, void *clientData)
{
	std::cerr << "A VERT IS CHANGED" << std::endl;

}

void FaceChanged(MUintArray componentIds[], unsigned int count, void *clientData)
{

}

void EdgeChanged(MUintArray componentIds[], unsigned int count, void *clientData)
{

}