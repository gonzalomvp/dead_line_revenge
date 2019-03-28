#include "common/stdafx.h"
#include "repeat.h"

void CRepeatNode::init(TiXmlElement* behaviorElem) {
	CDecoratorNode::init(behaviorElem);

	m_iTimes = -1;
	if (behaviorElem->Attribute("iTimes")) {
		m_iTimes = std::stoi(behaviorElem->Attribute("iTimes"));
	}
}

void CRepeatNode::onEnter() {
	m_iCounter = 0;
}

Status CRepeatNode::update(float step) {
	if (m_iCounter < m_iTimes || m_iTimes == -1) {
		Status s = m_pChildNode->tick(step);
		
		if (s == eFail) {
			return eFail;
		}
		else if (s == eSuccess) {
			++m_iCounter;
		}
		return eRunning;
	}

	return eSuccess;
}

