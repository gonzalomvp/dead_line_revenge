#pragma once

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
	Behavior(CBehaviorTreeComponent* owner) : mOwner(owner), mStatus(eInvalid) {}
	virtual ~Behavior() {}
	virtual Status tick(float step);

	Entity* getOwnerEntity();

protected:
	virtual Status update(float step) = 0;
	virtual void onEnter() {}
	virtual void onExit()  {}

	CBehaviorTreeComponent* mOwner;

private:
	Status mStatus;
};