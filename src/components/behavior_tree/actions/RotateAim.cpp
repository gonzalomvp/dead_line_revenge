#include "common/stdafx.h"
#include "RotateAim.h"

#include "entities/Entity.h"
#include "messages/Message.h"

void CRotateAim::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("fAngle"));
	m_fAngle = std::stof(_pBehaviorElem->Attribute("fAngle"));
}

CBehavior::EStatus CRotateAim::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);

	TMessageGetAimDirection messageGetAimDirection;
	pOwnerEntity->receiveMessage(&messageGetAimDirection);
	float fAngle = vangle(messageGetAimDirection.v2Dir);
	fAngle += DEG2RAD(m_fAngle) * _fDeltaTime;

	TMessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.v2Dir = vunit(fAngle);
	pOwnerEntity->receiveMessage(&messageSetAimDirection);

	return EStatus::ESuccess;
}