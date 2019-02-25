#include "common/stdafx.h"
#include "entities_factory.h"

#include "components/component.h"
#include "scenes/world.h"

Entity* EntitiesFactory::createEntity(Entity::TType type) {
	switch (type)
	{
	case Entity::EEnemyMelee:
		return EntitiesFactory::createEnemyMelee();
		break;
	case Entity::EEnemyBig:
		return EntitiesFactory::createEnemyBig();
		break;
	case Entity::EEnemyRange:
		return EntitiesFactory::createEnemyRange();
		break;
	case Entity::ETurret:
		break;
	case Entity::EBoss:
		break;
	case Entity::EPlayer:
		return createPlayer();
		break;
	case Entity::EPickup:
		break;
	case Entity::EWeapon:
		break;
	case Entity::EMine:
		break;
	case Entity::EExplossion:
		break;
	case Entity::ENuclearExplossion:
		break;
	case Entity::EHUDMessage:
		break;
	default:
		break;
	}
}

Entity* EntitiesFactory::createEnemyMelee() {
	Entity* enemy = NEW(Entity, Entity::EEnemyMelee);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_pWorld->m_mEnemyData[Entity::EEnemyMelee].v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyMelee].sImageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_pWorld->getPlayer(), g_pWorld->m_mEnemyData[Entity::EEnemyMelee].fSpeed, 0);
	aiMelee->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_pWorld->m_mEnemyData[Entity::EEnemyMelee].iCollisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyMelee].iLife, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyMelee].iPoints);
	points->init();
	return enemy;
}

Entity* EntitiesFactory::createEnemyBig() {
	Entity* enemy = NEW(Entity, Entity::EEnemyBig);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_pWorld->m_mEnemyData[Entity::EEnemyBig].v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyBig].sImageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_pWorld->getPlayer(), g_pWorld->m_mEnemyData[Entity::EEnemyBig].fSpeed, 0);
	aiMelee->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_pWorld->m_mEnemyData[Entity::EEnemyBig].iCollisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyBig].iLife, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyBig].iPoints);
	points->init();
	return enemy;
}

Entity* EntitiesFactory::createEnemyRange() {
	Entity* enemy = NEW(Entity, Entity::EEnemyRange);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_pWorld->m_mEnemyData[Entity::EEnemyRange].v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyRange].sImageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentWeapon* gun = NEW(ComponentWeapon, enemy, g_pWorld->m_mWeaponData[g_pWorld->m_mEnemyData[Entity::EEnemyRange].eWeapon]);
	gun->init();
	ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, g_pWorld->getPlayer());
	aiFire->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_pWorld->getPlayer(), g_pWorld->m_mEnemyData[Entity::EEnemyRange].fSpeed, 200);
	aiMelee->init();
	ComponentAIEvade* aiEvade = NEW(ComponentAIEvade, enemy, g_pWorld->getPlayer(), g_pWorld->m_mEnemyData[Entity::EEnemyRange].fSpeed, 150);
	aiEvade->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_pWorld->m_mEnemyData[Entity::EEnemyRange].iCollisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyRange].iLife, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_pWorld->m_mEnemyData[Entity::EEnemyRange].iPoints);
	points->init();
	return enemy;
}

Entity* EntitiesFactory::createPlayer() {
	Entity* player = NEW(Entity, Entity::EPlayer);
	ComponentTransform* transform = NEW(ComponentTransform, player, vmake(0.f, 0.f), vmake(30, 25));
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, player, "data/player.png", 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentPlayerController* playerControl = NEW(ComponentPlayerController, player);
	playerControl->init();
	ComponentMove* movement = NEW(ComponentMove, player, vmake(0.0f, 0.0f), g_pWorld->m_fPlayerSpeed, false, false);
	movement->init();
	ComponentWeapon* weapon = NEW(ComponentWeapon, player, g_pWorld->m_mWeaponData[ComponentWeapon::EEREVOLVER]);
	weapon->init();
	ComponentCollider* collider = NEW(ComponentCollider, player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayerCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EEnemyWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, player, g_pWorld->m_iPlayerLife, 0, 20);
	life->init();
	ComponentHUD* hudComponent = NEW(ComponentHUD, player);
	hudComponent->init();
	return player;
}