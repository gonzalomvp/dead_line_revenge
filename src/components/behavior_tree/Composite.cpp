#include "common/stdafx.h"
#include "Composite.h"

#include "components/BehaviorTreeComponent.h"

CComposite::~CComposite() {
	for (size_t i = 0; i < m_vChildBehaviors.size(); ++i) {
		delete m_vChildBehaviors[i];
	}
}

void CComposite::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);

	ASSERT(behaviorElem);
	TiXmlElement* pChildElem = behaviorElem->FirstChildElement();

	ASSERT(pChildElem, "Composite nodes must have at least one child behavior");

	for (pChildElem; pChildElem; pChildElem = pChildElem->NextSiblingElement()) {
		this->addChildBehavior(CBehavior::createBehaviorFromXML(m_pOwnerComponent, pChildElem));
	}
}

void CComposite::abort() {
	CBehavior::abort();

	for (size_t i = 0; i < m_vChildBehaviors.size(); ++i) {
		m_vChildBehaviors[i]->abort();
	}
}