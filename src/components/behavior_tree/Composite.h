#pragma once
#include "components/behavior_tree/Behavior.h"
#include <vector>

class CBehaviorTreeComponent;

class CComposite : public CBehavior {
public:
	CComposite(CBehaviorTreeComponent* owner) : CBehavior(owner) {}
	~CComposite();
	void AddBehavior(CBehavior* behavior) { mChildren.push_back(behavior); }

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort() override;

protected:
	typedef std::vector<CBehavior*> Behaviors;
	Behaviors mChildren;
};