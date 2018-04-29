#pragma once

#include "components/component.h"

struct Message;

class Entity {
public:
	enum TType {
		EPlayer,
		EPickup,
		EEnemy,
		ETurret,
		EWeapon,
		EMine,
		EExplossion,
		ENuclearExplossion,
		EHUDMessage,
	};

	Entity(TType type) : m_type(type) {}
	~Entity();

	void activate();
	void deactivate();
	TType getType() { return m_type; }

	void run(float deltaTime);
	void receiveMessage(Message* message);
	void addComponent(Component* component);
	void removeComponent(Component* component) {}

	std::vector<Component*> m_components;
private:
	TType m_type;
};

// Game utils
