#pragma once
#include "behavior.h"
#include <vector>

class CBehaviorTreeComponent;

class Group : public Behavior {
public:
	Group(CBehaviorTreeComponent* owner) : Behavior(owner) {}
	~Group();
	void AddBehavior(Behavior* behavior) { mChildren.push_back(behavior); }

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors mChildren;
};