// Composite nodes
REG_BEHAVIOR(SelectorNode, "selector")
REG_BEHAVIOR(SequenceNode, "sequence")
REG_BEHAVIOR(RepeatNode,   "repeat")
REG_BEHAVIOR(ParallelNode, "parallel")

// Conditions
REG_BEHAVIOR(CheckLifeCondition,     "checkLife")
REG_BEHAVIOR(CheckDistanceCondition, "checkDistance")

// Actions
REG_BEHAVIOR(ChangeWeaponAction,       "changeWeapon")
REG_BEHAVIOR(GoToRandomPositionAction, "goToRandomPosition")
REG_BEHAVIOR(GoToPlayerPositionAction, "goToPlayerPosition")
REG_BEHAVIOR(AimToPlayerAction,        "aimToPlayer")
REG_BEHAVIOR(FireAction,               "fire")
REG_BEHAVIOR(WaitAction,               "wait")
REG_BEHAVIOR(RotateAimAction,          "rotateAim")
REG_BEHAVIOR(ChangeAimDirectionAction, "changeAimDirection")
REG_BEHAVIOR(ChangeSpriteAction,       "changeSprite")
REG_BEHAVIOR(ChangeSpeedAction,        "changeSpeed")
