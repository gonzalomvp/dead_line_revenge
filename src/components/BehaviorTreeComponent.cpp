#include "common/stdafx.h"
#include "BehaviorTreeComponent.h"

// Composites
#include "components/behavior_tree/composites/Parallel.h"
#include "components/behavior_tree/composites/Selector.h"
#include "components/behavior_tree/composites/Sequence.h"

// Decorators
#include "components/behavior_tree/decorators/Repeat.h"

// Conditions
#include "components/behavior_tree/conditions/CheckIsAtDistance.h"
#include "components/behavior_tree/conditions/CheckLife.h"

// Actions
#include "components/behavior_tree/actions/Aim.h"
#include "components/behavior_tree/actions/CalculateFleePosition.h"
#include "components/behavior_tree/actions/CalculateRandomPosition.h"
#include "components/behavior_tree/actions/ChangeSpeed.h"
#include "components/behavior_tree/actions/ChangeSprite.h"
#include "components/behavior_tree/actions/ChangeWeapon.h"
#include "components/behavior_tree/actions/Fire.h"
#include "components/behavior_tree/actions/GoToBlackboardPosition.h"
#include "components/behavior_tree/actions/RotateAim.h"
#include "components/behavior_tree/actions/Wait.h"

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

CBehaviorTreeComponent::~CBehaviorTreeComponent() {
	delete m_pRootBehavior;
}

bool CBehaviorTreeComponent::loadFromXML(const char* _psFilename) {
	ASSERT(_psFilename);
	TiXmlDocument doc(_psFilename);
	if (!doc.LoadFile()) {
		fprintf(stderr, "Couldn't read behavior tree configuration from %s", _psFilename);
		return false;
	}
	TiXmlHandle hDoc(&doc);
	m_pRootBehavior = createBehaviorFromXML(hDoc.FirstChild("root").FirstChild().Element());

	return true;
}

void CBehaviorTreeComponent::run(float deltaTime)
{
	Component::run(deltaTime);
	if (!m_isActive)
		return;
	
	if (m_pRootBehavior) {
		m_pRootBehavior->run(deltaTime);
	}
}

CBehavior* CBehaviorTreeComponent::createBehaviorFromXML(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);
	CBehavior* behavior = nullptr;

	std::string type = behaviorElem->Value();
	CBehavior::EType eBehaviorType = CBehavior::getBehaviorTypeByName(type);

	switch (eBehaviorType) {
#define REG_BEHAVIOR(val) \
		case CBehavior::E##val: \
			behavior = new C##val(this); \
			behavior->init(behaviorElem); \
			break;
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		default:
			ASSERT(false, "Invalid Behavior type: %s", type.c_str());
			break;
	}

	return behavior;
}