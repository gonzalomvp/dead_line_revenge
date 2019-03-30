#include "common/stdafx.h"
#include "Composite.h"

#include "components/BehaviorTreeComponent.h"

CComposite::~CComposite() {
	for (size_t i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}

void CComposite::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	TiXmlElement* childElem = behaviorElem->FirstChildElement();
	for (childElem; childElem; childElem = childElem->NextSiblingElement()) {
		this->AddBehavior(mOwner->createBehaviorFromXML(childElem));
	}
}

void CComposite::abort() {
	CBehavior::abort();
	for (size_t i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->abort();
	}
}