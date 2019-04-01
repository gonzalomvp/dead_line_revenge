#pragma once

class  CComponent;
struct TMessage;

class CEntity {
public:
	class IListener {
	public:
		virtual void onEntityDestroyed(CEntity* _pEntity) = 0;
	};

	enum EType {
#define REG_ENTITY(val, name) \
		E##val,
#include "REG_ENTITIES.h"
#undef REG_ENTITY

		EInvalid,
	};
	static const int NUM_ENTITIES = EInvalid;
	static CEntity::EType getEntityTypeByName(const std::string& name);

	CEntity(EType _eType, const vec2& _v2Pos, const vec2& _v2Size)
	: m_eType(_eType)
	, m_v2Pos(_v2Pos)
	, m_v2Size(_v2Size)
	{}

	~CEntity();

	void  activate();
	void  deactivate();

	void run           (float _fDeltaTime      );
	void receiveMessage(TMessage* _pMessage    );
	void addComponent  (CComponent* _pComponent);

	virtual void registerToDestroy  (IListener* _pListener);
	virtual void unregisterToDestroy(IListener* _pListener);

	EType getType() const        { return m_eType;     }
	vec2  getPos  () const       { return m_v2Pos;     }
	void  setPos  (vec2 _v2Pos)  { m_v2Pos = _v2Pos;   }
	vec2  getSize () const       { return m_v2Size;    }
	void  setSize (vec2 _v2Size) { m_v2Size = _v2Size; }

private:
	EType m_eType;
	vec2  m_v2Pos;
	vec2  m_v2Size;
	
	std::vector<CComponent*> m_vComponents;
	std::vector<IListener*>  m_vDestroyListeners;

	struct TEntityInfo {
		CEntity::EType eType;
		std::string    sName;
	};
	static TEntityInfo s_aEntityInfo[];
};
