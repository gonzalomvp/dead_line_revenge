#include "common/stdafx.h"
#include "hit.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"

void Hit::onEnter() {
	mHitTimer = 0;
	//mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	//mOwner->getCharacter()->SetAngularVelocity(0.0f);
}

Status Hit::update(float step) {
	mHitTimer += step;
	if (mHitTimer >= 0.5f) {
		return eSuccess;
	}
	else {
		return eRunning;
	}
}

void Hit::onExit() {
	//mOwner->getCharacter()->SetIsHit(false);
	//mOwner->getCharacter()->SetLife(mOwner->getCharacter()->GetLife() - 1.0f);
}