#pragma once

class BehaviorTree;

enum Status {
	eInvalid,
	eSuccess,
	eFail,
	eRunning,
};

class Behavior {
public:
	Behavior(BehaviorTree* owner) : mOwner(owner), mStatus(eInvalid) {}
	virtual ~Behavior() {}
	virtual Status tick(float step);

protected:
	virtual Status update(float step) = 0;
	virtual void onEnter() {}
	virtual void onExit()  {}

	BehaviorTree* mOwner;

private:
	Status mStatus;
};