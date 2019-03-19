#pragma once
#include "behavior.h"
#include <vector>

class CBehaviorNodeTreeComponent;

class CGroupNode : public CBehaviorNode {
public:
	CGroupNode(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}
	~CGroupNode();
	void AddBehavior(CBehaviorNode* behavior) { mChildren.push_back(behavior); }

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort() override;

protected:
	typedef std::vector<CBehaviorNode*> Behaviors;
	Behaviors mChildren;
};