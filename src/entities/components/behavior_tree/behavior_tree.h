#pragma once

#include "entities/components/component.h"
#include "blackboard.h"

class Entity;
class Behavior;
class TiXmlElement;

class BehaviorTree : public Component {
public:
	BehaviorTree(Entity* owner) : Component(owner), mRootBehavior(nullptr) {}
	~BehaviorTree();
	
	Entity* getCharacter() const { return m_owner; }
	CBlackboard& getBlackboard() { return m_oBlackboard; }
	
	bool load(const char* filename);
	Behavior* createBehavior(TiXmlElement* behaviorElem);
	virtual void run(float deltaTime);

private:
	Behavior* mRootBehavior;
	CBlackboard m_oBlackboard;
};