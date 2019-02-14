#include "common/stdafx.h"

#include "blackboard.h"

void CBlackboard::setValueVec2(TKey key, const vec2& _vValue) {
	m_mapVec2[key] = _vValue;
}

void CBlackboard::setValueInt(TKey key, int _iValue) {
	m_mapInt[key] = _iValue;
}

void CBlackboard::setValueFloat(TKey key, float _fValue) {
	m_mapFloat[key] = _fValue;
}

void CBlackboard::setValueEntity(TKey key, ptr<Entity> _pValue) {
	ASSERT(_pValue);
	m_mapEntity[key] = _pValue;
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

bool CBlackboard::getValueEntity(TKey key, ptr<Entity>& pValue_) {
	bool bRet = false;
	auto it = m_mapEntity.find(key);
	if (it != m_mapEntity.end())
	{
		bRet = true;
		pValue_ = m_mapEntity[key];
	}

	return bRet;
}