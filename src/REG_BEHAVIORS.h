// Composites
REG_BEHAVIOR(SelectorNode, "Selector")
REG_BEHAVIOR(SequenceNode, "Sequence")
REG_BEHAVIOR(ParallelNode, "Parallel")

// Decorators
REG_BEHAVIOR(RepeatNode,   "Repeat")

// Conditions
REG_BEHAVIOR(CheckLifeCondition,         "CheckLife")
REG_BEHAVIOR(CheckIsAtDistanceCondition, "CheckIsAtDistance")

// Actions
REG_BEHAVIOR(ChangeWeaponAction,            "ChangeWeapon")
REG_BEHAVIOR(CalculateRandomPositionAction, "CalculateRandomPosition")
REG_BEHAVIOR(GoToPlayerPositionAction,      "GoToPlayerPosition")
REG_BEHAVIOR(GoToBlackboardPositionAction,  "GoToBlackboardPosition")
REG_BEHAVIOR(AimTo,                         "Aim")
REG_BEHAVIOR(FireAction,                    "Fire")
REG_BEHAVIOR(WaitAction,                    "Wait")
REG_BEHAVIOR(RotateAimAction,               "RotateAim")
REG_BEHAVIOR(ChangeAimDirectionAction,      "ChangeAimDirection")
REG_BEHAVIOR(ChangeSpriteAction,            "ChangeSprite")
REG_BEHAVIOR(ChangeSpeedAction,             "ChangeSpeed")
REG_BEHAVIOR(CalculateFleePositionAction,   "CalculateFleePosition")
