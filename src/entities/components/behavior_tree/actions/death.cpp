#include "common/stdafx.h"
#include "death.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"

void Death::onEnter() {
	mRespawnTimer = 0;
	//mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	//mOwner->getCharacter()->SetAngularVelocity(0.0f);
}

Status Death::update(float step) {
	mRespawnTimer += step;
	if (mRespawnTimer >= 5.0f) {
		return eSuccess;
	}
	else {
		return eRunning;
	}
}

void Death::onExit() {
	//mOwner->getCharacter()->SetLife(5.0f);
	//mOwner->getCharacter()->SetLoc(USVec2D(0, 0));
}