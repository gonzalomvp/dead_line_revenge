#include "common/stdafx.h"
#include "Condition.h"

#include <sstream>

void CCondition::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	m_bNegate = false;
	if (_pBehaviorElem->Attribute("bNegate")) {
		std::istringstream is(_pBehaviorElem->Attribute("bNegate"));
		is >> std::boolalpha >> m_bNegate;
	}
}

CBehavior::EStatus CCondition::onUpdate(float _fDeltaTime) {
	if (check(_fDeltaTime) != m_bNegate) {
		return EStatus::ESuccess;
	}
	else {
		return EStatus::EFail;
	}
}
