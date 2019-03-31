#include "common/stdafx.h"
#include "CheckLife.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/BehaviorTreeComponent.h"

void CCheckLife::init(TiXmlElement* _pBehaviorElem) {
	CCondition::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("iLife"));
	m_iLife = std::stoi(_pBehaviorElem->Attribute("iLife"));
}

bool CCheckLife::check(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	
	ASSERT(pOwnerEntity);

	TMessageGetLife messageGetLife;
	pOwnerEntity->receiveMessage(&messageGetLife);

	return messageGetLife.iCurrentLife >= m_iLife;
}