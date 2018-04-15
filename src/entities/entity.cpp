#include "../common/stdafx.h"
#include "entity.h"
#include "./components/component.h"
#include "../scenes/world.h"


Entity::~Entity() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		delete *itComponents;
	}
}

void Entity::activate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->activate();
	}
}

void Entity::deactivate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->deactivate();
	}
}

void Entity::run() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->run();
	}
}

void Entity::addComponent(Component* component) {
	m_components.push_back(component);
}

void Entity::setPos(vec2 pos) {
	m_pos = pos;
	m_sprite->setPos(m_pos);
}

void Entity::move(vec2 speed) {
	setPos(vadd(m_pos, speed));
}

void Entity::receiveMessage(Message* message) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->receiveMessage(message);
	}
}

EntityPlayer::EntityPlayer(int i) {
	m_pos  = vmake(i, i);
	m_size = vmake(20, 20);
	m_coolDown = 0.0f;
	m_dashCoolDown = 0.0f;
	
	//m_sprite = new Sprite(m_pos, m_size, g_graphicsEngine->getTexture("data/player.png"));
	g_graphicsEngine->addGfxEntity(m_sprite);
	//m_targetSprite = new Sprite(m_targetPos, vmake(50, 50), g_graphicsEngine->getTexture("data/target.png"));
	g_graphicsEngine->addGfxEntity(m_targetSprite);

	setTargetPos(vadd(m_pos, vmake(-500, -500)));
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
}

EntityPlayer::~EntityPlayer() {
	//no se esta llamando porque ahora todo son entidades genericas
	g_graphicsEngine->removeGfxEntity(m_targetSprite);
	g_inputManager->unregisterEvent(this);
}

void EntityPlayer::run() {
	++m_coolDown;
	++m_dashCoolDown;
	if (m_dashCoolDown >= 8.0f) {
		m_speed = vmake(0.0f, 0.0f);
	}
	move(m_speed);
}

void EntityPlayer::setTargetPos(vec2 targetPos) {
	m_targetPos = targetPos;
	m_targetSprite->setPos(m_targetPos);
}

bool EntityPlayer::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EKey) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		switch (keyEvent.key)
		{
		case 'A': move(vmake(-5, 0));
			break;
		case 'D': move(vmake(5, 0));
			break;
		case 'W': move(vmake(0, 5));
			break;
		case 'S': move(vmake(0, -5));
			break;
		}
	}

	if (eventType == IInputManager::TEvent::EMouse) {
		const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
		setTargetPos(vmake(mouseEvent.x, mouseEvent.y));
		if (mouseEvent.buttonMask == 1) {
			fire();
		}
		if (mouseEvent.buttonMask == 2) {
			dash();
		}
	}

	return true;
}

void EntityPlayer::fire() {
	if (m_coolDown >= 20) {
		vec2 direction = vsub(m_targetPos, m_pos);
		vec2 bulletDir= vscale(direction, 1 / vlen(direction));
		EntityBullet* bullet = new EntityBullet(m_pos, bulletDir);
		g_world->addEntity(bullet);

		//float a = atan2(bulletDir.y, bulletDir.x - 1) + DEG2RAD(5);
		//printf("%f\n", RAD2DEG(a));
		//bulletDir = vunit(a);
		//bullet = new EntityBullet(vadd(m_pos, vscale(bulletDir,30)), bulletDir);
		//g_world->addEntity(bullet);
		//a = atan2(bulletDir.y, bulletDir.x - 1) - DEG2RAD(5);
		//printf("%f\n", RAD2DEG(a));
		//bulletDir = vunit(a);
		//bullet = new EntityBullet(m_pos, bulletDir);
		//g_world->addEntity(bullet);
		m_coolDown = 0.0f;
	}	
}

void EntityPlayer::dash() {
	if (m_dashCoolDown >=20.0f) {
		vec2 direction = vsub(m_targetPos, m_pos);
		m_speed = vscale(direction, 30 / vlen(direction));
		m_dashCoolDown = 0.0f;
	}
}

EntityBullet::EntityBullet(vec2 pos, vec2 dir) {
	m_pos = pos;
	m_size = vmake(10, 10);
	m_speed = vscale(dir, 6);
	m_sprite = new Sprite(g_graphicsEngine->getTexture("data/bullet.png"), 2);
	m_sprite->setPos(m_pos);
	m_sprite->setSize(m_size);
	g_graphicsEngine->addGfxEntity(m_sprite);
}
void EntityBullet::run() {
	move(m_speed);
}