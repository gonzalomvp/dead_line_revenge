#include "common/stdafx.h"
#include "HUDComponent.h"

#include "engine/graphics_engine.h"
#include "engine/text.h"
#include "engine/sprite.h"
#include "entities/entity.h"
#include "gui/string_manager.h"
#include "input/input_manager.h"
#include "messages/message.h"
#include "scenes/world.h"

namespace {
	const std::string s_sLifeText  = "LTEXT_GUI_LIFE_HUD";
	const std::string s_sScoreText = "LTEXT_GUI_SCORE_HUD";
	const std::string s_sAmmoText  = "LTEXT_GUI_AMMO_HUD";

	const char* s_psTargetImage = "data/target.png";
	const char* s_psReloadImage = "data/reload.png";
}

CHUDComponent::~CHUDComponent() {
	for (size_t i = 0; i < m_vGfxEntities.size(); ++i) {
		if (g_pGraphicsEngine) {
			g_pGraphicsEngine->removeGfxEntity(m_vGfxEntities[i]);
		}
		DELETE(m_vGfxEntities[i]);
	}

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseMove);
	}
}

void CHUDComponent::init() {
	CComponent::init();

	ASSERT(g_pGraphicsEngine && g_pStringManager && g_pInputManager);
	
	CText* pTitle = nullptr;
	float fTitleEndPos = 0.0f;

	// Life HUD
	pTitle = NEW(CText, s_sLifeText, vmake(fTitleEndPos + 20.0f, 450.0f), 4);
	g_pGraphicsEngine->addGfxEntity(pTitle);
	m_vGfxEntities.push_back(pTitle);
	fTitleEndPos = pTitle->getPos().x + g_pStringManager->calculateTextHalfWidth(s_sLifeText) * 2.0f;
	m_pLifeText = NEW(CText, "0", vmake(pTitle->getPos().x, 430.0f), 4);
	g_pGraphicsEngine->addGfxEntity(m_pLifeText);
	m_vGfxEntities.push_back(m_pLifeText);

	// Score HUD
	pTitle = NEW(CText, s_sScoreText, vmake(fTitleEndPos + 20.0f, 450.0f), 4);
	g_pGraphicsEngine->addGfxEntity(pTitle);
	m_vGfxEntities.push_back(pTitle);
	fTitleEndPos = pTitle->getPos().x + g_pStringManager->calculateTextHalfWidth(s_sScoreText) * 2.0f;
	m_pScoreText = NEW(CText, "0", vmake(pTitle->getPos().x, 430.0f), 4);
	g_pGraphicsEngine->addGfxEntity(m_pScoreText);
	m_vGfxEntities.push_back(m_pScoreText);

	// AMMO HUD
	pTitle = NEW(CText, s_sAmmoText, vmake(fTitleEndPos + 20.0f, 450.0f), 4);
	g_pGraphicsEngine->addGfxEntity(pTitle);
	m_vGfxEntities.push_back(pTitle);
	fTitleEndPos = pTitle->getPos().x + g_pStringManager->calculateTextHalfWidth(s_sAmmoText) * 2.0f;
	m_pAmmoText = NEW(CText, "0", vmake(pTitle->getPos().x, 430.0f), 4);
	g_pGraphicsEngine->addGfxEntity(m_pAmmoText);
	m_vGfxEntities.push_back(m_pAmmoText);

	// MESSAGE HUD
	m_pMessageText = NEW(CText, "", vmake((WORLD_WIDTH * 0.5f), 20.0f), 4);
	g_pGraphicsEngine->addGfxEntity(m_pMessageText);
	m_vGfxEntities.push_back(m_pMessageText);

	// Target HUD
	m_pTargetSprite = NEW(CSprite, g_pGraphicsEngine->getTexture(s_psTargetImage), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), 0.0f, 1.0f, 4);
	m_pTargetSprite->setSize(vmake(36.0f, 36.0f));
	g_pGraphicsEngine->addGfxEntity(m_pTargetSprite);
	m_vGfxEntities.push_back(m_pTargetSprite);

	// Reload Animation
	m_pReloadSprite = NEW(CSprite, g_pGraphicsEngine->getTexture(s_psReloadImage), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), 0.0f, 1.0f, 4);
	g_pGraphicsEngine->addGfxEntity(m_pReloadSprite);
	m_vGfxEntities.push_back(m_pReloadSprite);

	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseMove);
}

void CHUDComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && m_pMessageText && m_pLifeText && m_pScoreText && m_pAmmoText && m_pReloadSprite && g_pWorld);

	if (m_fMessageTimer > 0.0f) {
		m_fMessageTimer -= _fDeltaTime;
		if (m_fMessageTimer <= 0.0f) {
			m_pMessageText->setText("");
		}
	}

	MessageGetLife msgLife;
	m_pOwner->receiveMessage(&msgLife);
	m_pLifeText->setText(std::to_string(msgLife.currentLife));

	m_pScoreText->setText(std::to_string(g_pWorld->getScore()));

	MessageAmmoInfo msgAmmo;
	m_pOwner->receiveMessage(&msgAmmo);
	m_pAmmoText->setText(std::to_string(msgAmmo.currentAmmo) + "/" + std::to_string(msgAmmo.totalAmmo));

	vec2 v2Pos = m_pOwner->getPos();
	vec2 v2Size = m_pOwner->getSize();

	// Full sprite reload animation
	//m_pReloadSprite->setPos(vmake(v2Pos.x, v2Pos.y - v2Size.y * msgAmmo.reloadPercent * 0.5f));
	//m_pReloadSprite->setSize(vmake(v2Size.x, v2Size.y * (1.0f - msgAmmo.reloadPercent)));

	// Right sprite reload animation
	m_pReloadSprite->setPos(vmake(v2Pos.x + v2Size.x * 0.5f + 5.0f, v2Pos.y - v2Size.y * msgAmmo.reloadPercent * 0.5f));
	m_pReloadSprite->setSize(vmake(5, v2Size.y * (1 - msgAmmo.reloadPercent)));

	// Top sprite reload animation
	//m_pReloadSprite->setPos(vmake(v2Pos.x - v2Size.x * msgAmmo.reloadPercent * 0.5f, v2Pos.y + v2Size.y * 0.5f + 5));
	//m_pReloadSprite->setSize(vmake(v2Size.x * (1.0f - msgAmmo.reloadPercent), 5));
}

void CHUDComponent::receiveMessage(Message* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pMessageText);
	if (MessageShowHUDMessage* pMessage = dynamic_cast<MessageShowHUDMessage*>(_pMessage)) {
		m_pMessageText->setText(pMessage->message);
		m_pMessageText->setPos(vmake((WORLD_WIDTH * 0.5f) - g_pStringManager->calculateTextHalfWidth(pMessage->message), m_pMessageText->getPos().y));
		m_fMessageTimer = pMessage->time;
	}
}

bool CHUDComponent::onEvent(const IInputManager::CEvent& _event) {
	// We never want to consume this event
	bool bConsumed = false;
	if (!m_bIsActive)
		return bConsumed;
	
	ASSERT(m_pOwner && m_pTargetSprite);

	if (const IInputManager::CMouseEvent* pEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event)) {
		if (pEvent->getType() == IInputManager::EEventType::EMouseMove) {
			vec2 v2TargetPos = pEvent->getPos();
			m_pTargetSprite->setPos(v2TargetPos);

			MessageSetAimDirection messageSetAimDirection;
			messageSetAimDirection.direction = vnorm(vsub(v2TargetPos, m_pOwner->getPos()));
			m_pOwner->receiveMessage(&messageSetAimDirection);
		}
	}

	return bConsumed;
}