#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class ChangeSprite : public Behavior {
public:
	ChangeSprite(BehaviorTree* owner, std::string texture) : Behavior(owner), mTexture(texture) {}

protected:
	virtual Status update(float step) override;

private:
	std::string mTexture;
};

