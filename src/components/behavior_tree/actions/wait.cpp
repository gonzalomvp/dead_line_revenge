#include "common/stdafx.h"
#include "wait.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CWaitAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fTime"));
	m_fWaitTime = std::stof(behaviorElem->Attribute("fTime"));

	m_fRandomDeviation = 0.0f;
	if (behaviorElem->Attribute("fRandomDeviation")) {
		m_fRandomDeviation = std::stof(behaviorElem->Attribute("fRandomDeviation"));
	}
}

void CWaitAction::onEnter() {
	m_fWaitTimer = m_fWaitTime + CORE_FRand(-m_fRandomDeviation, m_fRandomDeviation);
}

Status CWaitAction::update(float step) {
	if (m_fWaitTimer <= 0.0f) {
		return eSuccess;
	}

	m_fWaitTimer -= step;

	return eRunning;
}