#include "common/stdafx.h"
#include "group.h"

#include "entities/components/behavior_tree/behavior_tree.h"

void Group::init(TiXmlElement* behaviorElem) {
	std::vector<Behavior*> childBehaviors;
	TiXmlElement* childElem = behaviorElem->FirstChildElement("behavior");
	for (childElem; childElem; childElem = childElem->NextSiblingElement()) {
		this->AddBehavior(mOwner->createBehaviorFromXML(childElem));
	}
}

Group::~Group() {
	for (size_t i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}