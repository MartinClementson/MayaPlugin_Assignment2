#pragma once
#include "includes.h"
#include "CallbackIds.h"


void TimeCallback(float elapsedTime, float lastTime, void *clientData)
{
	static float time = 0;
	time += elapsedTime;
	//std::cerr << "5 Second Callback is triggered" << "\n";
	//std::cerr << "Elapsed time : " << ceil(time) << "\n";

}


void attrChangedCB(MNodeMessage::AttributeMessage msg, MPlug & plug,
	MPlug & otherPlug, void*)
{
	//MNodeMessage::kAttributeEval
	if (msg & MNodeMessage::AttributeMessage::kAttributeEval && !plug.isArray() && plug.isElement()) //if a specific vert has changed
	{
		MStringArray changes;
		MFnNumericData point(plug.attribute());
		
		
		plug.getSetAttrCmds(changes, MPlug::kChanged);
		if (changes.length() == 1)
		{

			
			float x, y, z;
			point.getData3Float(x, y, z);
			x = plug.child(0).asFloat();
			y = plug.child(1).asFloat();
			z = plug.child(2).asFloat();
		std::cerr << "A Vert has changed!! |" << x << ","<< y <<"," << z  << "|  " << changes << std::endl;
		//plug.attribute().apiTypeStr()
		//std::cerr << "Attribute : " << plug.className() << std::endl;
		}
	}


	//plug.attribute().apiTypeStr()
}


void transformNodeChanged(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	MFnTransform obj(transformNode);
	MMatrix matrix = obj.transformationMatrix();
	std::cerr << matrix.matrix[0][0] << " " << matrix.matrix[0][1] << " " << matrix.matrix[0][2] << " " << matrix.matrix[0][3] << std::endl;
	std::cerr << matrix.matrix[1][0] << " " << matrix.matrix[1][1] << " " << matrix.matrix[1][2] << " " << matrix.matrix[1][3] << std::endl;
	std::cerr << matrix.matrix[2][0] << " " << matrix.matrix[2][1] << " " << matrix.matrix[2][2] << " " << matrix.matrix[2][3] << std::endl;
	std::cerr << matrix.matrix[3][0] << " " << matrix.matrix[3][1] << " " << matrix.matrix[3][2] << " " << matrix.matrix[3][3] << std::endl;

	std::cerr << "A TransformNode has changed!! |" << obj.name() << "   | "<< modified << std::endl;

}

void topologyChanged(MObject& node, void *clientData)
{
	MFnMesh thisMesh(node);

	std::cerr << "Topology has changed |" << thisMesh.name() << std::endl;
}

void NodeCreated(MObject &node, void *clientData)
{


	if (node.hasFn(MFn::kMesh))
	{

		MFnDagNode nodeHandle(node);
		MStatus result;
		MCallbackId polyId = MNodeMessage::addAttributeChangedCallback(node, attrChangedCB, NULL, &result);
		
		
		if (MS::kSuccess == result)
		{

			polyCallbackIds.append(polyId);
			std::cerr << "A new node was created \n";
			std::cerr << "Callback triggered from  node : " << nodeHandle.name() << "\n"
			<< "Node Path : " << nodeHandle.fullPathName() << "\n" << std::endl;
		
			MDagPath path = MDagPath::getAPathTo(node);
			polyId = MDagMessage::addWorldMatrixModifiedCallback(path, transformNodeChanged, NULL, &result);
			if (result == MS::kSuccess)
			{
				transformCallbackIds.append(polyId);
			}
			MCallbackId polyId = MPolyMessage::addPolyTopologyChangedCallback(node, topologyChanged, NULL, &result);
			if (result == MS::kSuccess)
			{
				polyCallbackIds.append(polyId);
				std::cerr << "Polychange callback added!  " << node.apiTypeStr() << std::endl;
			}

		}

		
		
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





