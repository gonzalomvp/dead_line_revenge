#pragma once

#include "entities/components/behavior_tree/behavior_tree.h"

//=============================================================================
// BossIAComponent class
//=============================================================================
class BossIAComponent : public CBehaviorNodeTreeComponent {
public:
	BossIAComponent(Entity* owner, const char* filename) :
		CBehaviorNodeTreeComponent(owner),
		m_btFilename(filename) {}

	virtual void init();

private:
	const char* m_btFilename;
};