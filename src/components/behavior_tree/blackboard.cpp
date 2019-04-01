#include "common/stdafx.h"
#include "Blackboard.h"

CBlackboard::~CBlackboard() {
	for (auto itEntity = m_mapEntity.begin(); itEntity != m_mapEntity.end(); ++itEntity) {
		itEntity->second->unregisterToDestroy(this);
	}
}

void CBlackboard::setValueVec2(std::string _sKey, const vec2& _vValue) {
	m_mapVec2[_sKey] = _vValue;
}

void CBlackboard::setValueInt(std::string _sKey, int _iValue) {
	m_mapInt[_sKey] = _iValue;
}

void CBlackboard::setValueFloat(std::string _sKey, float _fValue) {
	m_mapFloat[_sKey] = _fValue;
}

void CBlackboard::setValueEntity(std::string _sKey, CEntity* _pValue) {
	ASSERT(_pValue);

	clearValueEntity(_sKey);

	m_mapEntity[_sKey] = _pValue;
	_pValue->registerToDestroy(this);
}

bool CBlackboard::getValueVec2(std::string _sKey, vec2& vValue_) {
	bool bRet = false;
	if (m_mapVec2.count(_sKey) > 0) {
		bRet = true;
		vValue_ = m_mapVec2[_sKey];
	}
	return bRet;
}

bool CBlackboard::getValueInt(std::string _sKey, int& iValue_) {
	bool bRet = false;
	if (m_mapInt.count(_sKey) > 0) {
		bRet = true;
		iValue_ = m_mapInt[_sKey];
	}
	return bRet;
}

bool CBlackboard::getValueFloat(std::string _sKey, float& fValue_) {
	bool bRet = false;
	if (m_mapFloat.count(_sKey) > 0) {
		bRet = true;
		fValue_ = m_mapFloat[_sKey];
	}
	return bRet;
}

bool CBlackboard::getValueEntity(std::string _sKey, CEntity*& pValue_) {
	bool bRet = false;
	if (m_mapEntity.count(_sKey) > 0) {
		bRet = true;
		pValue_ = m_mapEntity[_sKey];
	}
	return bRet;
}

void CBlackboard::clearValueVec2(std::string _sKey) {
	m_mapVec2.erase(_sKey);
}

void CBlackboard::clearValueInt(std::string _sKey) {
	m_mapInt.erase(_sKey);
}

void CBlackboard::clearValueFloat(std::string _sKey) {
	m_mapFloat.erase(_sKey);
}

void CBlackboard::clearValueEntity(std::string _sKey) {
	if (m_mapEntity.count(_sKey) > 0 && m_mapEntity[_sKey]) {
		m_mapEntity[_sKey]->unregisterToDestroy(this);
	}
	m_mapEntity.erase(_sKey);
}

void CBlackboard::onEntityDestroyed(CEntity* _pEntity) {
	auto it = m_mapEntity.begin();
	while(it != m_mapEntity.end()) {
		if (it->second == _pEntity) {
			it = m_mapEntity.erase(it);
		}
		else {
			++it;
		}
	}
}
