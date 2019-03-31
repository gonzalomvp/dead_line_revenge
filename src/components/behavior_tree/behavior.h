#pragma once

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

class CBehaviorTreeComponent;
class CEntity;

enum EStatus {
	EInvalid,
	ESuccess,
	EFail,
	EAborted,
	ERunning,
};

class CBehavior {
public:
	enum EType {
#define REG_BEHAVIOR(val) \
		E##val,
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		EInvalid,
	};

	static const int NUM_BEHAVIORS = EType::EInvalid;
	static EType getBehaviorTypeByName(const std::string& name);

	static CBehavior* createBehaviorFromXML(TiXmlElement* _pBehaviorElem, CBehaviorTreeComponent* _pOwnerComponent);

	CBehavior(CBehaviorTreeComponent* owner) : mOwner(owner), mStatus(EStatus::EInvalid) {}
	virtual ~CBehavior() {}

	virtual void init(TiXmlElement* behaviorElem) {}
	virtual void abort() { mStatus = EStatus::EAborted; }

	virtual EStatus run(float step);

	EStatus getStatus() const { return mStatus; }
	CEntity* getOwnerEntity();

protected:
	virtual EStatus onUpdate(float step) = 0;
	virtual void onEnter()  {}
	virtual void onExit ()  {}

	CBehaviorTreeComponent* mOwner;

private:
	struct SBehaviorInfo {
		EType eType;
		std::string sName;
	};
	static SBehaviorInfo s_aBehaviorInfo[];

	EStatus mStatus;
};