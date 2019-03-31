#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CAim : public CBehavior {
public:
	CAim(CBehaviorTreeComponent* owner) : CBehavior(owner), m_sBlackboardkey("") {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sBlackboardkey;
};

