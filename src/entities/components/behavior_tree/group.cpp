#include "common/stdafx.h"
#include "group.h"

#include "entities/components/behavior_tree/behavior_tree.h"

void CGroupNode::init(TiXmlElement* behaviorElem) {
	TiXmlElement* childElem = behaviorElem->FirstChildElement("behavior");
	for (childElem; childElem; childElem = childElem->NextSiblingElement()) {
		this->AddBehavior(mOwner->createBehaviorFromXML(childElem));
	}
}

CGroupNode::~CGroupNode() {
	for (size_t i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}