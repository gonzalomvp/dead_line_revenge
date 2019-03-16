#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class ChangeSprite : public CBehaviorNode {
public:
	ChangeSprite(CBehaviorNodeTreeComponent* owner, std::string texture) : CBehaviorNode(owner), mTexture(texture) {}

protected:
	virtual Status update(float step) override;

private:
	std::string mTexture;
};

