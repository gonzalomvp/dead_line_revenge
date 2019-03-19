#pragma once

#include "components/component.h"
#include "blackboard.h"

class CBehaviorNode;
class TiXmlElement;

class CBehaviorNodeTreeComponent : public Component {
public:

	CBehaviorNodeTreeComponent(Entity* _pOwner) : Component(_pOwner), m_pRootBehavior(nullptr) {}
	~CBehaviorNodeTreeComponent();
	
	bool         loadFromXML(const char* _psFilename);
	CBlackboard& getBlackboard() { return m_blackboard; }
	CBehaviorNode*   createBehaviorFromXML(TiXmlElement* _pBehaviorElem);

	// CComponent
	virtual void run(float _fDeltaTime) override;
  
private:
	

	CBehaviorNode *   m_pRootBehavior;
	CBlackboard   m_blackboard;
};