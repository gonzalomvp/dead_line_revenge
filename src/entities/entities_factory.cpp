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
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_world->m_enemyData[Entity::EEnemyMelee].size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_world->m_enemyData[Entity::EEnemyMelee].imageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_world->getPlayer(), g_world->m_enemyData[Entity::EEnemyMelee].speed, 0);
	aiMelee->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_world->m_enemyData[Entity::EEnemyMelee].collisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_world->m_enemyData[Entity::EEnemyMelee].life, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_world->m_enemyData[Entity::EEnemyMelee].points);
	points->init();
	return enemy;
}

Entity* EntitiesFactory::createEnemyBig() {
	Entity* enemy = NEW(Entity, Entity::EEnemyBig);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_world->m_enemyData[Entity::EEnemyBig].size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_world->m_enemyData[Entity::EEnemyBig].imageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_world->getPlayer(), g_world->m_enemyData[Entity::EEnemyBig].speed, 0);
	aiMelee->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_world->m_enemyData[Entity::EEnemyBig].collisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_world->m_enemyData[Entity::EEnemyBig].life, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_world->m_enemyData[Entity::EEnemyBig].points);
	points->init();
	return enemy;
}

Entity* EntitiesFactory::createEnemyRange() {
	Entity* enemy = NEW(Entity, Entity::EEnemyRange);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, vmake(0.f, 0.f), g_world->m_enemyData[Entity::EEnemyRange].size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, g_world->m_enemyData[Entity::EEnemyRange].imageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentWeapon* gun = NEW(ComponentWeapon, enemy, g_world->m_weaponData[g_world->m_enemyData[Entity::EEnemyRange].weapon]);
	gun->init();
	ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, g_world->getPlayer());
	aiFire->init();
	ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, g_world->getPlayer(), g_world->m_enemyData[Entity::EEnemyRange].speed, 200);
	aiMelee->init();
	ComponentAIEvade* aiEvade = NEW(ComponentAIEvade, enemy, g_world->getPlayer(), g_world->m_enemyData[Entity::EEnemyRange].speed, 150);
	aiEvade->init();
	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, g_world->m_enemyData[Entity::EEnemyRange].collisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, g_world->m_enemyData[Entity::EEnemyRange].life, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, g_world->m_enemyData[Entity::EEnemyRange].points);
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
	ComponentMove* movement = NEW(ComponentMove, player, vmake(0.0f, 0.0f), g_world->m_playerSpeed, false, false);
	movement->init();
	ComponentWeapon* weapon = NEW(ComponentWeapon, player, g_world->m_weaponData[ComponentWeapon::ERevolver]);
	weapon->init();
	ComponentCollider* collider = NEW(ComponentCollider, player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayerCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EEnemyWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, player, g_world->m_playerLife, 0, 20);
	life->init();
	ComponentHUD* hudComponent = NEW(ComponentHUD, player);
	hudComponent->init();
	return player;
}