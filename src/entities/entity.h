#pragma once

class  Component;
struct Message;

class Entity {
public:
	enum TType {
#define REG_ENTITY(val, name) \
		E##val,
#include "REG_ENTITIES.h"
#undef REG_ENTITY

		EInvalid,
	};

	static const int NUM_ENTITIES = EInvalid;

	Entity(TType type) : m_type(type) {}
	~Entity();

	void  activate  ();
	void  deactivate();
	TType getType   () const { return m_type; }

	void run            (float deltaTime);
	void receiveMessage (Message* message);
	void addComponent   (Component* component);
private:
	TType                       m_type;
	std::vector<Component*> m_components;
};
