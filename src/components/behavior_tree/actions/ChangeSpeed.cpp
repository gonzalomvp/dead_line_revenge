#include "common/stdafx.h"
#include "ChangeSpeed.h"

#include "entities/Entity.h"
#include "messages/Message.h"

void CChangeSpeed::init(TiXmlElement* _pOwnerComponent) {
	CBehavior::init(_pOwnerComponent);
	ASSERT(_pOwnerComponent);

	ASSERT(_pOwnerComponent->Attribute("fSpeed"));
	m_fSpeed = std::stof(_pOwnerComponent->Attribute("fSpeed"));
}

CBehavior::EStatus CChangeSpeed::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);
	TMessageSetMovementSpeed messageSetMovementSpeed;
	messageSetMovementSpeed.fSpeed = m_fSpeed;
	pOwnerEntity->receiveMessage(&messageSetMovementSpeed);

	return EStatus::ESuccess;
}