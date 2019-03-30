#include "common/stdafx.h"
#include "DecoratorNode.h"

#include "components/behavior_tree/behavior_tree.h"

void CDecoratorNode::init(TiXmlElement* behaviorElem) {
	TiXmlElement* childElem = behaviorElem->FirstChildElement();

	ASSERT(childElem->NextSibling() == nullptr, "Decorator nodes must have only one child behavior");

	m_pChildNode = mOwner->createBehaviorFromXML(childElem);
}

CDecoratorNode::~CDecoratorNode() {
		delete m_pChildNode;
}

void CDecoratorNode::abort() {
	m_pChildNode->abort();
}