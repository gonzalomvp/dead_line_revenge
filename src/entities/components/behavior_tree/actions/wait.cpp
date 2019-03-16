#include "common/stdafx.h"
#include "wait.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "entities/components/behavior_tree/behavior_tree.h"

void CWaitAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CWaitAction must have 1 param");
	m_fWaitTime = std::stof(vParams[0]);
}

void CWaitAction::onEnter() {
	m_fWaitTimer = 0;
}

Status CWaitAction::update(float step) {
	if (m_fWaitTimer >= m_fWaitTime) {
		return eSuccess;
	}

	m_fWaitTimer += step;

	return eRunning;
}