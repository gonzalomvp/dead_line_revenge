#include "common/stdafx.h"
#include "Repeat.h"

void CRepeat::init(TiXmlElement* _pBehaviorElem) {
	CDecorator::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	m_iRepeatTimes = -1;
	if (_pBehaviorElem->Attribute("iRepeatTimes")) {
		m_iRepeatTimes = std::stoi(_pBehaviorElem->Attribute("iRepeatTimes"));
	}
}

void CRepeat::onEnter() {
	CDecorator::onEnter();

	m_iCounter = m_iRepeatTimes;
}

CBehavior::EStatus CRepeat::onUpdate(float _fDeltaTime) {
	if (m_iCounter != 0) {
		EStatus eChildStatus = m_pChildBehavior->run(_fDeltaTime);
		if (eChildStatus != EStatus::ESuccess) {
			return eChildStatus;
		}
		if (m_iCounter > 0) {
			--m_iCounter;
		}	
	}

	if (m_iCounter == 0) {
		return EStatus::ESuccess;
	}
	else {
		return EStatus::ERunning;
	}
}

