#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CChangeWeapon : public CBehavior {
public:
	CChangeWeapon(CBehaviorTreeComponent* owner) : CBehavior(owner) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual EStatus onUpdate(float step) override;

private:
	std::string mWeaponName;
};

