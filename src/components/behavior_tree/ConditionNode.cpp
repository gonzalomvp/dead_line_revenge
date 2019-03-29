#include "common/stdafx.h"
#include "ConditionNode.h"

#include <sstream>

void CConditionNode::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	m_bNegate = false;
	if (behaviorElem->Attribute("bNegate")) {
		std::istringstream is(behaviorElem->Attribute("bNegate"));
		is >> std::boolalpha >> m_bNegate;
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
