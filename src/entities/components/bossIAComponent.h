#pragma once

#include "entities/components/behavior_tree/behavior_tree.h"

//=============================================================================
// BossIAComponent class
//=============================================================================
class BossIAComponent : public BehaviorTree {
public:
	BossIAComponent(ptr<Entity> owner, const char* filename) :
		BehaviorTree(owner),
		m_btFilename(filename) {}

	virtual void init();

private:
	const char* m_btFilename;
};