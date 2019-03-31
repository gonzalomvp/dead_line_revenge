#pragma once

class  CComponent;
struct TMessage;

class Entity {
public:
	class IListener {
	public:
		virtual void onEntityDestroyed(Entity* _pEntity) = 0;
	};

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
	void receiveMessage (TMessage* message);
	void addComponent   (CComponent* component);

	virtual void registerToDestroy(IListener* _pListener);
	virtual void unregisterToDestroy(IListener* _pListener);

protected:
	vec2 m_v2Pos;
	vec2 m_v2Size;
	EType                       m_type;
	std::vector<CComponent*> m_components;

	std::vector<IListener*> m_vlisteners;
};
