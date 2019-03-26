#include "common/stdafx.h"
#include "behavior_tree.h"

#include "components/behavior_tree/behavior.h"
#include "components/behavior_tree/selector.h"
#include "components/behavior_tree/sequence.h"
#include "components/behavior_tree/repeat.h"
#include "components/behavior_tree/parallel.h"
#include "components/behavior_tree/actions/change_sprite.h"
#include "components/behavior_tree/actions/hit.h"
#include "components/behavior_tree/actions/idle.h"
#include "components/behavior_tree/actions/chase.h"
#include "components/behavior_tree/conditions/check_hit.h"
#include "components/behavior_tree/actions/death.h"
#include "components/behavior_tree/conditions/check_dead.h"
#include "components/behavior_tree/conditions/check_distance.h"
#include "components/behavior_tree/actions/goto_random.h"
#include "components/behavior_tree/actions/goto_player.h"
#include "components/behavior_tree/actions/wait.h"
#include "components/behavior_tree/actions/fire.h"
#include "components/behavior_tree/actions/change_weapon.h"
#include "components/behavior_tree/actions/check_life.h"
#include "components/behavior_tree/actions/rotate_aim.h"
#include "components/behavior_tree/actions/aim_player.h"
#include "components/behavior_tree/actions/change_aim_direction.h"
#include "components/behavior_tree/actions/change_speed.h"

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

CBehaviorNodeTreeComponent::~CBehaviorNodeTreeComponent() {
	delete m_pRootBehavior;
}

bool CBehaviorNodeTreeComponent::loadFromXML(const char* _psFilename) {
	ASSERT(_psFilename);
	TiXmlDocument doc(_psFilename);
	if (!doc.LoadFile()) {
		fprintf(stderr, "Couldn't read behavior tree configuration from %s", _psFilename);
		return false;
	}
	TiXmlHandle hDoc(&doc);
	m_pRootBehavior = createBehaviorFromXML(hDoc.FirstChild("root").FirstChild("behavior").Element());

	return true;
}

void CBehaviorNodeTreeComponent::run(float deltaTime)
{
	Component::run(deltaTime);
	if (!m_isActive)
		return;
	
	if (m_pRootBehavior) {
		m_pRootBehavior->tick(deltaTime);
	}
}

CBehaviorNode* CBehaviorNodeTreeComponent::createBehaviorFromXML(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);
	CBehaviorNode* behavior = nullptr;
	
	ASSERT(behaviorElem->Attribute("type"), "Missing type in Behavior element");
	std::string type = behaviorElem->Attribute("type");
	CBehaviorNode::EType eBehaviorType = CBehaviorNode::getBehaviorTypeByName(type);

	switch (eBehaviorType) {
#define REG_BEHAVIOR(val, name) \
		case CBehaviorNode::E##val: \
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