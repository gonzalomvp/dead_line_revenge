#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CChangeSprite : public CBehavior {
public:
	CChangeSprite(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_sTextureFile("")
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sTextureFile;
};