#pragma once
#include "behavior.h"
#include <vector>

class BehaviorTree;

class Group : public Behavior {
public:
	Group(BehaviorTree* owner) : Behavior(owner) {}
	~Group();
	void AddBehavior(Behavior* behavior) { mChildren.push_back(behavior); }

protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors mChildren;
};