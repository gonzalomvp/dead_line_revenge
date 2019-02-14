#pragma once

#include "entities/components/component.h"
#include "blackboard.h"

class Entity;
class Behavior;
class TiXmlElement;

class BehaviorTree : public Component {
public:
	BehaviorTree(ptr<Entity> owner) : Component(owner), mRootBehavior(nullptr) {}
	~BehaviorTree();
	
	ptr<Entity> getCharacter() const { return m_owner; }
	CBlackboard& getBlackboard() { return m_oBlackboard; }
	
	bool load(const char* filename);
	ptr<Behavior> createBehavior(ptr<TiXmlElement> behaviorElem);
	virtual void run(float deltaTime);

private:
	ptr<Behavior> mRootBehavior;
	CBlackboard m_oBlackboard;
};