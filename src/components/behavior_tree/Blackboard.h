#pragma once

#include "entities/Entity.h"

#include <map>
#include <string>

class CEntity;

class CBlackboard : public CEntity::IListener {
public:
	CBlackboard() {}
	~CBlackboard();

	void setValueVec2  (std::string _sKey, const vec2& _v2Value);
	void setValueInt   (std::string _sKey, int _iValue);
	void setValueFloat (std::string _sKey, float _fValue);
	void setValueEntity(std::string _sKey, CEntity* _pValue);

	bool getValueVec2  (std::string _sKey, vec2& v2Value_);
	bool getValueInt   (std::string _sKey, int& iValue_ );
	bool getValueFloat (std::string _sKey, float& fValue_);
	bool getValueEntity(std::string _sKey, CEntity*& pValue_);

	void clearValueVec2  (std::string _sKey);
	void clearValueInt   (std::string _sKey);
	void clearValueFloat (std::string _sKey);
	void clearValueEntity(std::string _sKey);

	// CEntity::IListener
	virtual void onEntityDestroyed(CEntity* _pEntity) override;

private:
	std::map<std::string, int>      m_mapInt;
	std::map<std::string, float>    m_mapFloat;
	std::map<std::string, vec2>     m_mapVec2;
	std::map<std::string, CEntity*>	m_mapEntity;
};
