#pragma once

#include "entities/Entity.h"

#include <map>
#include <string>

class CEntity;

class CBlackboard : public CEntity::IListener {
public:
	typedef std::string TKey;

	CBlackboard() {}
	~CBlackboard();

	void setValueVec2(TKey key, const vec2& _vValue);
	void setValueInt(TKey key, int _iValue);
	void setValueFloat(TKey key, float _fValue);
	void setValueEntity(TKey key, CEntity* _pValue);

	bool getValueVec2(TKey key, vec2& vValue_);
	bool getValueInt(TKey key, int& iValue_ );
	bool getValueFloat(TKey key, float& fValue_);
	bool getValueEntity(TKey key, CEntity*& pValue_);

	// CEntity::IListener
	virtual void onEntityDestroyed(CEntity* _pEntity) override;

private:
	std::map<TKey, int>		    m_mapInt;
	std::map<TKey, float>		m_mapFloat;
	std::map<TKey, vec2>		m_mapVec2;
	std::map<TKey, CEntity*>	m_mapEntity;
};