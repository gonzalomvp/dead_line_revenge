#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CChangeSpriteAction : public CBehaviorNode {
public:
	CChangeSpriteAction(CBehaviorNodeTreeComponent* _pOwner) : CBehaviorNode(_pOwner) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	std::string m_sTextureFile;
};

