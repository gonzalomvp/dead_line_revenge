#pragma once

#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CDecoratorNode : public CBehaviorNode {
public:
	CDecoratorNode(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}
	~CDecoratorNode();

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort() override;

protected:
	CBehaviorNode* m_pChildNode;
};