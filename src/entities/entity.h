#pragma once

#include "rapidjson/document.h"

using namespace rapidjson;

class  Component;
struct Message;

class Entity {
public:
	enum TType {
		EEnemyMelee,
		EEnemyBig,
		EEnemyRange,
		ETurret,
		EPlayer,
		EPickup,
		EWeapon,
		EMine,
		EExplossion,
		ENuclearExplossion,
		EHUDMessage,
	};

	Entity(TType type) : m_type(type) {}
	~Entity();

	void  activate  ();
	void  deactivate();
	TType getType   () const { return m_type; }

	void run            (float deltaTime);
	void receiveMessage (ptr<Message> message);
	void addComponent   (ptr<Component> component);

	void loadComponents(ptr<const rapidjson::Value> pInfo);

private:
	TType                       m_type;
	std::vector<ptr<Component>> m_components;
};
