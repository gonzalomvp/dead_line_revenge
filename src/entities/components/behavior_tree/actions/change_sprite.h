#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class ChangeSprite : public Behavior {
public:
	ChangeSprite(CBehaviorTreeComponent* owner, std::string texture) : Behavior(owner), mTexture(texture) {}

protected:
	virtual Status update(float step) override;

private:
	std::string mTexture;
};

