#pragma once

class  Component;
struct Message;

class Entity {
public:
	enum EType {
#define REG_ENTITY(val, name) \
		E##val,
#include "REG_ENTITIES.h"
#undef REG_ENTITY

		EInvalid,
	};

	static const int NUM_ENTITIES = EInvalid;

	Entity(EType type) : m_type(type) {}
	~Entity();

	void  activate  ();
	void  deactivate();
	EType getType   () const { return m_type; }

	void run            (float deltaTime);
	void receiveMessage (Message* message);
	void addComponent   (Component* component);
private:
	EType                       m_type;
	std::vector<Component*> m_components;
};
