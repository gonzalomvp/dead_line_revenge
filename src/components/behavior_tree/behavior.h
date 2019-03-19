#pragma once

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

class CBehaviorNodeTreeComponent;
class Entity;

enum Status {
	eInvalid,
	eSuccess,
	eFail,
	eAborted,
	eRunning,
};

class CBehaviorNode {
public:
	enum EType {
#define REG_BEHAVIOR(val, name) \
		E##val,
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		EInvalid,
	};

	static const int NUM_BEHAVIORS = EInvalid;
	static EType getBehaviorTypeByName(const std::string& name);

	CBehaviorNode(CBehaviorNodeTreeComponent* owner) : mOwner(owner), mStatus(eInvalid) {}
	virtual ~CBehaviorNode() {}

	virtual void init(TiXmlElement* behaviorElem) {}
	virtual void abort() { mStatus = eAborted; }

	virtual Status tick(float step);

	Status getStatus() const { return mStatus; }
	Entity* getOwnerEntity();

protected:
	virtual Status update(float step) = 0;
	
	virtual void onEnter()  {}
	virtual void onExit ()  {}

	CBehaviorNodeTreeComponent* mOwner;

private:
	struct SBehaviorInfo {
		EType eType;
		std::string sName;
	};
	static SBehaviorInfo s_aBehaviorInfo[];

	Status mStatus;
};