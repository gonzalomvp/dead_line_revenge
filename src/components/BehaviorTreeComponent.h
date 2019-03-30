#pragma once

#include "components/AIComponent.h"
#include "components/behavior_tree/Blackboard.h"

class CBehavior;
class TiXmlElement;

class CBehaviorTreeComponent : public CAIComponent {
public:

	CBehaviorTreeComponent(Entity* _pOwner) : CAIComponent(_pOwner), m_pRootBehavior(nullptr) {}
	~CBehaviorTreeComponent();
	
	bool         loadFromXML(const char* _psFilename);
	CBlackboard& getBlackboard() { return m_blackboard; }
	CBehavior*   createBehaviorFromXML(TiXmlElement* _pBehaviorElem);

	// CComponent
	virtual void run(float _fDeltaTime) override;
  
private:
	

	CBehavior *   m_pRootBehavior;
	CBlackboard   m_blackboard;
};