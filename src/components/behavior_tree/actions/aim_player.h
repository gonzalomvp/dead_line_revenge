#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CAimTo : public CBehaviorNode {
public:
	CAimTo(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_sBlackboardkey("") {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	std::string m_sBlackboardkey;
};

