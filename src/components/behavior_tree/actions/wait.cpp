#include "common/stdafx.h"
#include "Wait.h"

void CWait::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("fTime"));
	m_fWaitTime = std::stof(_pBehaviorElem->Attribute("fTime"));

	m_fRandomDeviation = 0.0f;
	if (_pBehaviorElem->Attribute("fRandomDeviation")) {
		m_fRandomDeviation = std::stof(_pBehaviorElem->Attribute("fRandomDeviation"));
	}
}

void CWait::onEnter() {
	CBehavior::onEnter();

	m_fWaitTimer = m_fWaitTime + CORE_FRand(-m_fRandomDeviation, m_fRandomDeviation);
}

CBehavior::EStatus CWait::onUpdate(float _fDeltaTime) {
	if (m_fWaitTimer <= 0.0f) {
		return EStatus::ESuccess;
	}
	m_fWaitTimer -= _fDeltaTime;

	return EStatus::ERunning;
}