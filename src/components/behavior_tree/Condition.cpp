#include "common/stdafx.h"
#include "Condition.h"

#include <sstream>

void CCondition::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	CBehavior::init(behaviorElem);

	m_bNegate = false;
	if (behaviorElem->Attribute("bNegate")) {
		std::istringstream is(behaviorElem->Attribute("bNegate"));
		is >> std::boolalpha >> m_bNegate;
	}
}

EStatus CCondition::onUpdate(float step) {
	if (check(step) != m_bNegate) {
		return EStatus::ESuccess;
	}
	else {
		return EStatus::EFail;
	}
}
