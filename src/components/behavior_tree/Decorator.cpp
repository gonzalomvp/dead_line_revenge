#include "common/stdafx.h"
#include "Decorator.h"

#include "components/BehaviorTreeComponent.h"

CDecorator::~CDecorator() {
	delete m_pChildBehavior;
}

void CDecorator::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	TiXmlElement* pChildElem = _pBehaviorElem->FirstChildElement();

	ASSERT(pChildElem && pChildElem->NextSibling() == nullptr, "Decorator nodes must have only one child behavior");

	m_pChildBehavior = CBehavior::createBehaviorFromXML(m_pOwnerComponent, pChildElem);
}

void CDecorator::abort() {
	CBehavior::abort();

	m_pChildBehavior->abort();
}