#include "common/stdafx.h"
#include "Repeat.h"

void CRepeat::init(TiXmlElement* behaviorElem) {
	CDecorator::init(behaviorElem);

	m_iTimes = -1;
	if (behaviorElem->Attribute("iTimes")) {
		m_iTimes = std::stoi(behaviorElem->Attribute("iTimes"));
	}
}

void CRepeat::onEnter() {
	m_iCounter = 0;
}

CBehavior::EStatus CRepeat::onUpdate(float step) {
	if (m_iCounter < m_iTimes || m_iTimes == -1) {
		EStatus s = m_pChildNode->run(step);
		
		if (s == EStatus::EFail) {
			return EStatus::EFail;
		}
		else if (s == EStatus::ESuccess) {
			++m_iCounter;
		}
		return EStatus::ERunning;
	}

	return EStatus::ESuccess;
}

