#include "common/stdafx.h"
#include "MovementComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CMovementComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	if (vlen2(m_vDirection) != 0) {
		MessageGetTransform msgGetTransform;
		m_owner->receiveMessage(&msgGetTransform);

		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(msgGetTransform.pos, vscale(vnorm(m_vDirection), m_fSpeed));
		msgSetTransform.size = msgGetTransform.size;
		m_owner->receiveMessage(&msgSetTransform);
	}

	if (!m_bHasInertia) {
		m_vDirection = vmake(0.0f, 0.0f);
	}
}

void CMovementComponent::receiveMessage(Message* _pMessage) {
	if (!m_isActive)
		return;

	MessageAddMovement* msgAddMovement = dynamic_cast<MessageAddMovement*>(_pMessage);
	if (msgAddMovement) {
		m_vDirection = vadd(m_vDirection, msgAddMovement->dir);
	}
	else {
		MessageCheckCollision* msgCheckCollision = dynamic_cast<MessageCheckCollision*>(_pMessage);
		if (msgCheckCollision && msgCheckCollision->overlap && m_bHasBounce) {
			m_vDirection.x *= -1.f * msgCheckCollision->bounceX;
			m_vDirection.y *= -1.f * msgCheckCollision->bounceY;
		}
	}
}

Component* CMovementComponent::loadComponent(Entity* _pOwner, const rapidjson::Value* _pComponentInfo) {
	ASSERT(_pComponentInfo);

	// Default values
	float fSpeed = 1.0f;
	vec2  vDirection = vmake(0.0f, 0.0f);
	bool  bHasInertia = true;
	bool  bHasBounce = true;

	if ((*_pComponentInfo).HasMember("speed")) {
		fSpeed = (*_pComponentInfo)["speed"].GetFloat();
	}

	if ((*_pComponentInfo).HasMember("moveDirection")) {
		vDirection.x = (*_pComponentInfo)["moveDirection"]["x"].GetFloat();
		vDirection.y = (*_pComponentInfo)["moveDirection"]["y"].GetFloat();
	}

	if ((*_pComponentInfo).HasMember("hasInertia")) {
		bHasInertia = (*_pComponentInfo)["hasInertia"].GetBool();
	}

	if ((*_pComponentInfo).HasMember("hasBounce")) {
		bHasBounce = (*_pComponentInfo)["hasBounce"].GetBool();
	}

	Component* pComponent = NEW(CMovementComponent, _pOwner, vDirection, fSpeed, bHasInertia, bHasBounce);

	ASSERT(pComponent);
	pComponent->init();

	return pComponent;
}