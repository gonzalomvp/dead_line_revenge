#pragma once

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

class CBehaviorTreeComponent;
class CEntity;

class CBehavior {
public:
	enum EStatus {
		EInvalid,
		ESuccess,
		EFail,
		EAborted,
		ERunning,
	};

	enum EType {
#define REG_BEHAVIOR(val) \
		E##val,
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		EUnknown,
	};
	static const int NUM_BEHAVIORS = EType::EUnknown;
	static EType getBehaviorTypeByName(const std::string& name);

	static CBehavior* createBehaviorFromXML(CBehaviorTreeComponent* _pOwnerComponent, TiXmlElement* _pBehaviorElem);

	CBehavior(CBehaviorTreeComponent* _pOwnerComponent) : m_pOwnerComponent(_pOwnerComponent), m_eStatus(EStatus::EInvalid) {}
	virtual ~CBehavior() {}

	virtual void init(TiXmlElement* behaviorElem) {}
	virtual void abort() { m_eStatus = EStatus::EAborted; }

	virtual EStatus run(float step);

	EStatus  getStatus() const { return m_eStatus; }
	CEntity* getOwnerEntity();

protected:
	virtual EStatus onUpdate(float step) = 0;
	virtual void onEnter()  {}
	virtual void onExit ()  {}

	EStatus                 m_eStatus;
	CBehaviorTreeComponent* m_pOwnerComponent;

private:
	struct SBehaviorInfo {
		EType       eType;
		std::string sName;
	};
	static SBehaviorInfo s_aBehaviorInfo[];
};