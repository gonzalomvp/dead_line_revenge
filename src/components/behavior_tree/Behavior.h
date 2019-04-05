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
		ERunning,
		EAborted,
	};

	enum EType {
#define REG_BEHAVIOR(val) \
		E##val,
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		EUnknown,
	};
	static const int NUM_BEHAVIORS = EType::EUnknown;
	static EType getBehaviorTypeByName(const std::string& _sName);

	static CBehavior* createBehaviorFromXML(CBehaviorTreeComponent* _pOwnerComponent, TiXmlElement* _pBehaviorElem);

	CBehavior(CBehaviorTreeComponent* _pOwnerComponent) : m_pOwnerComponent(_pOwnerComponent), m_eStatus(EStatus::EInvalid) {}
	virtual ~CBehavior() {}

	EStatus run(float _fDeltaTime);

	EStatus  getStatus() const { return m_eStatus; }
	CEntity* getOwnerEntity() const;

	virtual void init(TiXmlElement* _pBehaviorElem) {}
	virtual void abort() { m_eStatus = EStatus::EAborted; }

protected:
	virtual EStatus onUpdate(float _fDeltaTime) = 0;
	virtual void onEnter() {}
	virtual void onExit () {}

	EStatus                 m_eStatus;
	CBehaviorTreeComponent* m_pOwnerComponent;

private:
	struct SBehaviorInfo {
		EType       eType;
		std::string sName;
	};
	static SBehaviorInfo s_aBehaviorInfo[];
};