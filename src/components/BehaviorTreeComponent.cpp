#include "common/stdafx.h"
#include "BehaviorTreeComponent.h"

#include "components/behavior_tree/Behavior.h"
#include "scenes/world.h"

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

CBehaviorTreeComponent::~CBehaviorTreeComponent() {
	delete m_pRootBehavior;
}

void CBehaviorTreeComponent::init() {
	CAIComponent::init();

	// Load the BT from an XML file
	ASSERT(m_psBTFile);
	TiXmlDocument doc(m_psBTFile);
	ASSERT(doc.LoadFile(), "Couldn't read behavior tree configuration from %s", m_psBTFile);
	TiXmlHandle hDoc(&doc);
	ASSERT(hDoc.FirstChild("root").Element(), "Mising root node in %s", m_psBTFile);
	ASSERT(hDoc.FirstChild("root").FirstChild().Element(), "Empty BT %s", m_psBTFile);

	m_pRootBehavior = CBehavior::createBehaviorFromXML(hDoc.FirstChild("root").FirstChild().Element(), this);

	// If there is a player save it in the Blackboard
	if (g_pWorld->getPlayer()) {
		m_blackboard.setValueEntity("player", g_pWorld->getPlayer());
	}
}

void CBehaviorTreeComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;
	
	if (m_pRootBehavior) {
		m_pRootBehavior->run(_fDeltaTime);
	}
}
