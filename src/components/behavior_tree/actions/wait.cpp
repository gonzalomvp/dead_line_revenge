#include "common/stdafx.h"
#include "Wait.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/BehaviorTreeComponent.h"

void CWait::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fTime"));
	m_fWaitTime = std::stof(behaviorElem->Attribute("fTime"));

	m_fRandomDeviation = 0.0f;
	if (behaviorElem->Attribute("fRandomDeviation")) {
		m_fRandomDeviation = std::stof(behaviorElem->Attribute("fRandomDeviation"));
	}
}

void CWait::onEnter() {
	m_fWaitTimer = m_fWaitTime + CORE_FRand(-m_fRandomDeviation, m_fRandomDeviation);
}

CBehavior::EStatus CWait::onUpdate(float step) {
	if (m_fWaitTimer <= 0.0f) {
		return EStatus::ESuccess;
	}

	m_fWaitTimer -= step;

	return EStatus::ERunning;
}