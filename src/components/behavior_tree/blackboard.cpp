#include "common/stdafx.h"

#include "Blackboard.h"

CBlackboard::~CBlackboard() {
	for (auto itEntity = m_mapEntity.begin(); itEntity != m_mapEntity.end(); ++itEntity) {
		itEntity->second->unregisterToDestroy(this);
	}
}

void CBlackboard::setValueVec2(TKey key, const vec2& _vValue) {
	m_mapVec2[key] = _vValue;
}

void CBlackboard::setValueInt(TKey key, int _iValue) {
	m_mapInt[key] = _iValue;
}

void CBlackboard::setValueFloat(TKey key, float _fValue) {
	m_mapFloat[key] = _fValue;
}

void CBlackboard::setValueEntity(TKey key, Entity* _pValue) {
	ASSERT(_pValue);

	if (m_mapEntity.count(key) > 0 && m_mapEntity[key]) {
		m_mapEntity[key]->unregisterToDestroy(this);
	}

	m_mapEntity[key] = _pValue;
	_pValue->registerToDestroy(this);
}

bool CBlackboard::getValueVec2(TKey key, vec2& vValue_) {
	bool bRet = false;
	auto it = m_mapVec2.find(key);
	if (it != m_mapVec2.end())
	{
		bRet = true;
		vValue_ = m_mapVec2[key];
	}

	return bRet;
}

bool CBlackboard::getValueInt(TKey key, int& iValue_) {
	bool bRet = false;
	auto it = m_mapInt.find(key);
	if (it != m_mapInt.end())
	{
		bRet = true;
		iValue_ = m_mapInt[key];
	}

	return bRet;
}

bool CBlackboard::getValueFloat(TKey key, float& fValue_) {
	bool bRet = false;
	auto it = m_mapFloat.find(key);
	if (it != m_mapFloat.end())
	{
		bRet = true;
		fValue_ = m_mapFloat[key];
	}

	return bRet;
}

bool CBlackboard::getValueEntity(TKey key, Entity*& pValue_) {
	bool bRet = false;
	auto it = m_mapEntity.find(key);
	if (it != m_mapEntity.end())
	{
		bRet = true;
		pValue_ = m_mapEntity[key];
	}

	return bRet;
}

void CBlackboard::onEntityDestroyed(Entity* _pEntity) {
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
