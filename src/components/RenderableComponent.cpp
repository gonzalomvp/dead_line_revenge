#include "common/stdafx.h"
#include "RenderableComponent.h"

#include "engine/graphics_engine.h"
#include "engine/sprite.h"
#include "entities/entity.h"
#include "messages/message.h"

CRenderableComponent::~CRenderableComponent() {
	DELETE(m_pSprite);
}

void CRenderableComponent::init() {
	Component::init();

	ASSERT(g_pGraphicsEngine);
	m_pSprite = NEW(CSprite, g_pGraphicsEngine->getTexture(m_sTexture), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), m_fAngle, m_fAlpha, m_iPriority);
	g_pGraphicsEngine->addGfxEntity(m_pSprite);
}

void CRenderableComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && m_pSprite);

	// Update pos and size
	m_pSprite->setPos(m_pOwner->getPos());
	m_pSprite->setSize(m_pOwner->getSize());

	// Update angle
	vec2 v2Orientation = vmake(0.0f, 0.0f);
	if (m_bAlignToMovement) {
		MessageGetMovementDir messageGetMovementDirection;
		m_pOwner->receiveMessage(&messageGetMovementDirection);
		v2Orientation = messageGetMovementDirection.dir;
		
	}
	if (m_bAlignToAim && vlen2(v2Orientation) == 0.0f) {
		MessageGetAimDirection messageGetAimDirection;
		m_pOwner->receiveMessage(&messageGetAimDirection);
		v2Orientation = messageGetAimDirection.direction;
	}
	if (vlen2(v2Orientation) != 0.0f) {
		m_pSprite->setAngle(vangle(v2Orientation));
	}

	// Blink effect when receiving a hit
	if (m_iHitTimer > 0) {
		--m_iHitTimer;
		int iHitAnim = m_iHitTimer % 2;
		if (iHitAnim) {
			m_pSprite->deactivate();
		}
		else {
			m_pSprite->activate();
		}
		if (m_iHitTimer == 0) {
			m_pSprite->activate();
		}
	}
}

void CRenderableComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pSprite && g_pGraphicsEngine);

	if (MessageChangeLife* pMessage = dynamic_cast<MessageChangeLife*>(_pMessage)) {
		if (m_iHitTimer <= 0 && pMessage->deltaLife < 0) {
			m_iHitTimer = m_iHitTime;
		}
	}
	else if (MessageChangeSprite* pMessage = dynamic_cast<MessageChangeSprite*>(_pMessage)) {
		ASSERT(g_pGraphicsEngine);
		g_pGraphicsEngine->removeGfxEntity(m_pSprite);
		DELETE(m_pSprite);

		m_sTexture = pMessage->texture;
		m_pSprite = NEW(CSprite, g_pGraphicsEngine->getTexture(m_sTexture), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), m_fAngle, m_fAlpha, m_iPriority);
		g_pGraphicsEngine->addGfxEntity(m_pSprite);
	}
}
