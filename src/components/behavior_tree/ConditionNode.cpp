#include "common/stdafx.h"
#include "ConditionNode.h"

void CConditionNode::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	m_bNegate = false;
	if (behaviorElem->Attribute("bNegate")) {
		m_bNegate = std::stoi(behaviorElem->Attribute("bNegate"));
	}
}

Status CConditionNode::update(float step) {
	if (check(step) != m_bNegate) {
		return eSuccess;
	}
	else {
		return eFail;
	}
}
