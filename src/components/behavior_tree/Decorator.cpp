#include "common/stdafx.h"
#include "Decorator.h"

#include "components/BehaviorTreeComponent.h"

CDecorator::~CDecorator() {
	delete m_pChildNode;
}

void CDecorator::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);

	TiXmlElement* childElem = behaviorElem->FirstChildElement();

	ASSERT(childElem && childElem->NextSibling() == nullptr, "Decorator nodes must have only one child behavior");

	m_pChildNode = CBehavior::createBehaviorFromXML(childElem, mOwner);
}

void CDecorator::abort() {
	CBehavior::abort();
	m_pChildNode->abort();
}