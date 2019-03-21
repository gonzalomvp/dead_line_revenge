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
	if (!m_isActive)
		return;

	ASSERT(m_owner);

	// Update pos and size based on entity transform
	MessageGetTransform msgGetTransform;
	m_owner->receiveMessage(&msgGetTransform);
	m_pSprite->setPos(msgGetTransform.pos);
	m_pSprite->setSize(msgGetTransform.size);

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
	ASSERT(_pMessage);

	if (!m_isActive)
		return;

	if (MessageChangeLife* pMessage = dynamic_cast<MessageChangeLife*>(_pMessage)) {
		if (m_iHitTimer <= 0 && pMessage->deltaLife < 0) {
			m_iHitTimer = m_iHitTime;
		}
	}
	else if (MessageSetAimDirection* pMessage = dynamic_cast<MessageSetAimDirection*>(_pMessage)) {
		if (pMessage->changeAngle) {
			m_pSprite->setAngle(vangle(pMessage->direction));
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
