#include "common/stdafx.h"
#include "repeat.h"

void CRepeatNode::init(TiXmlElement* behaviorElem) {
	CGroupNode::init(behaviorElem);

	ASSERT(mChildren.size() == 1, "Repeat decorators must only have one child behavior node");

	ASSERT(behaviorElem->Attribute("times"), "Missing times attribute in CRepeatNode");
	m_iTimes = std::stoi(behaviorElem->Attribute("times"));
}

void CRepeatNode::onEnter() {
	m_iCounter = 0;
}

Status CRepeatNode::update(float step) {
	if (m_iCounter < m_iTimes || m_iTimes == -1) {
		Status s = mChildren[0]->tick(step);
		
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

