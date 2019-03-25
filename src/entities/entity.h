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

	Entity(EType type, const vec2& _v2Pos, const vec2& _v2Size)
	: m_type(type)
	, m_v2Pos(_v2Pos)
	, m_v2Size(_v2Size)
	{}
	~Entity();

	void  activate  ();
	void  deactivate();

	EType getType   () const { return m_type; }

	vec2 getPos () const       { return m_v2Pos;     }
	void setPos (vec2 _v2Pos)  { m_v2Pos = _v2Pos;   }
	vec2 getSize() const       { return m_v2Size;    }
	void setSize(vec2 _v2Size) { m_v2Size = _v2Size; }

	void run            (float deltaTime);
	void receiveMessage (Message* message);
	void addComponent   (Component* component);

protected:
	vec2 m_v2Pos;
	vec2 m_v2Size;
	EType                       m_type;
	std::vector<Component*> m_components;
};
