#pragma once

#pragma pack(push)
#pragma pack()
#include "tinyxml/tinyxml.h"
#pragma pack(pop)

class CBehaviorTreeComponent;
class Entity;

enum Status {
	eInvalid,
	eSuccess,
	eFail,
	eRunning,
};

class Behavior {
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

	Behavior(CBehaviorTreeComponent* owner) : mOwner(owner), mStatus(eInvalid) {}
	virtual ~Behavior() {}

	virtual void init(TiXmlElement* behaviorElem) {}

	virtual Status tick(float step);

	Entity* getOwnerEntity();

protected:
	virtual Status update(float step) = 0;
	virtual void onEnter() {}
	virtual void onExit()  {}

	CBehaviorTreeComponent* mOwner;

private:
	struct SBehaviorInfo {
		EType eType;
		std::string sName;
	};
	static SBehaviorInfo s_aBehaviorInfo[];

	Status mStatus;
};