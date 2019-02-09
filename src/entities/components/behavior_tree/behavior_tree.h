#pragma once

#include "entities/components/component.h"

class Entity;
class Behavior;
class TiXmlElement;

class BehaviorTree : public Component {
public:
	BehaviorTree(ptr<Entity> owner) : Component(owner), mRootBehavior(nullptr) {}
	~BehaviorTree();
	
	ptr<Entity> getCharacter() const { return m_owner; }
	
	bool load(const char* filename);
	ptr<Behavior> createBehavior(ptr<TiXmlElement> behaviorElem);
	virtual void run(float deltaTime);

private:
	ptr<Behavior> mRootBehavior;
};