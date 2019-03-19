// Composite nodes
REG_BEHAVIOR_NODE(SelectorNode, "selector")
REG_BEHAVIOR_NODE(SequenceNode, "sequence")
REG_BEHAVIOR_NODE(RepeatNode,   "repeat")
REG_BEHAVIOR_NODE(ParallelNode, "parallel")

// Conditions
REG_BEHAVIOR_CONDITION(CheckLifeCondition, "checkLife")

// Actions
REG_BEHAVIOR_ACTION(ChangeWeaponAction,       "changeWeapon")
REG_BEHAVIOR_ACTION(GoToRandomPositionAction, "goToRandomPosition")
REG_BEHAVIOR_ACTION(GoToPlayerPositionAction, "goToPlayerPosition")
REG_BEHAVIOR_ACTION(AimToPlayerAction,        "aimToPlayer")
REG_BEHAVIOR_ACTION(FireAction,               "fire")
REG_BEHAVIOR_ACTION(WaitAction,               "wait")
REG_BEHAVIOR_ACTION(RotateAimAction,          "rotateAim")
REG_BEHAVIOR_ACTION(ChangeSpriteAction,       "changeSprite")
