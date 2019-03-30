#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CChangeSprite : public CBehavior {
public:
	CChangeSprite(CBehaviorTreeComponent* _pOwner) : CBehavior(_pOwner) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual EStatus onUpdate(float step) override;

private:
	std::string m_sTextureFile;
};

