#pragma once

#include "entities/components/component.h"
#include "blackboard.h"

class Behavior;
class TiXmlElement;

class CBehaviorTreeComponent : public Component {
public:
	CBehaviorTreeComponent(Entity* _pOwner) : Component(_pOwner), m_pRootBehavior(nullptr) {}
	~CBehaviorTreeComponent();
	
	bool         loadFromXML(const char* _psFilename);
	CBlackboard& getBlackboard() { return m_blackboard; }

	// CComponent
	virtual void run(float _fDeltaTime) override;
  
private:
	Behavior* createBehavior(TiXmlElement* _pBehaviorElem);

	Behavior*   m_pRootBehavior;
	CBlackboard m_blackboard;
};