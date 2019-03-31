#include "common/stdafx.h"
#include "RenderableComponent.h"

#include "engine/GraphicEngine.h"
#include "engine/Sprite.h"
#include "entities/Entity.h"
#include "messages/Message.h"

CRenderableComponent::~CRenderableComponent() {
	DELETE(m_pSprite);
}

void CRenderableComponent::init() {
	CComponent::init();

	ASSERT(g_pGraphicEngine);
	m_pSprite = NEW(CSprite, g_pGraphicEngine->getTexture(m_sTexture), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), m_fAngle, m_fAlpha, m_iPriority);
	g_pGraphicEngine->addGfxEntity(m_pSprite);
}

void CRenderableComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && m_pSprite);

	// Update pos and size
	m_pSprite->setPos(m_pOwner->getPos());
	m_pSprite->setSize(m_pOwner->getSize());

	// Update angle
	vec2 v2Orientation = vmake(0.0f, 0.0f);
	if (m_bAlignToMovement) {
		TMessageGetMovementDir messageGetMovementDirection;
		m_pOwner->receiveMessage(&messageGetMovementDirection);
		v2Orientation = messageGetMovementDirection.dir;
		
	}
	if (m_bAlignToAim && vlen2(v2Orientation) == 0.0f) {
		TMessageGetAimDirection messageGetAimDirection;
		m_pOwner->receiveMessage(&messageGetAimDirection);
		v2Orientation = messageGetAimDirection.direction;
	}
	if (vlen2(v2Orientation) != 0.0f) {
		m_pSprite->setAngle(RAD2DEG(vangle(v2Orientation)));
	}

	// Blink effect when receiving a hit
	if (m_fHitTimer > 0.0f) {
		m_fHitTimer -= _fDeltaTime;
		if (m_pSprite->isActive()) {
			m_pSprite->deactivate();
		}
		else {
			m_pSprite->activate();
		}
		if (m_fHitTimer <= 0.0f) {
			m_pSprite->activate();
		}
	}
}

void CRenderableComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pSprite && g_pGraphicEngine);

	if (TMessageChangeLife* pMessage = dynamic_cast<TMessageChangeLife*>(_pMessage)) {
		if (m_fHitTimer <= 0.0f && pMessage->deltaLife < 0) {
			m_fHitTimer = m_fHitTime;
		}
	}
	else if (TMessageChangeSprite* pMessage = dynamic_cast<TMessageChangeSprite*>(_pMessage)) {
		ASSERT(g_pGraphicEngine);
		g_pGraphicEngine->removeGfxEntity(m_pSprite);
		DELETE(m_pSprite);

		m_sTexture = pMessage->texture;
		m_pSprite = NEW(CSprite, g_pGraphicEngine->getTexture(m_sTexture), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), m_fAngle, m_fAlpha, m_iPriority);
		g_pGraphicEngine->addGfxEntity(m_pSprite);
	}
}
