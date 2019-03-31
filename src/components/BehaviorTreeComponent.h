#pragma once

#include "components/AIComponent.h"

#include "components/behavior_tree/Blackboard.h"

class CBehavior;
class TiXmlElement;

class CBehaviorTreeComponent : public CAIComponent {
public:
	CBehaviorTreeComponent(CEntity* _pOwner, const char* _psBTFile)
	: CAIComponent(_pOwner)
	, m_psBTFile(_psBTFile)
	, m_pRootBehavior(nullptr)
	{}
	
	~CBehaviorTreeComponent();
	
	CBlackboard& getBlackboard() { return m_blackboard; }

	// CComponent
	virtual void init()                 override;
	virtual void run(float _fDeltaTime) override;
  
private:
	const char*   m_psBTFile;
	CBehavior *   m_pRootBehavior;
	CBlackboard   m_blackboard;
};