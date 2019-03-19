#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CChangeWeaponAction : public CBehaviorNode {
public:
	CChangeWeaponAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	std::string mWeaponName;
};

