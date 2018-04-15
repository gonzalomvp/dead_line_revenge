#pragma once
#include "../engine/graphics_engine.h"
#include "../input/input_manager.h"

class Component;
struct Message;
class Sprite;

class Entity {
public:
	~Entity();

	void activate();
	void deactivate();


	void setPos(vec2 pos);
	void move(vec2 speed);
	virtual void run();
	void receiveMessage(Message* message);
	void addComponent(Component* component);
	void removeComponent(Component* component) {}

	vec2    m_pos;
	vec2    m_speed;
	vec2    m_size;
	Sprite* m_sprite;
	std::vector<Component*> m_components;
};

class EntityPlayer : public Entity, public IInputManager::IListener {
public:
	EntityPlayer(int i);
	~EntityPlayer();
	void setTargetPos(vec2 targetPos);
	void fire();
	void dash();
	virtual void run();
	virtual bool onEvent(const IInputManager::Event&);

private:
	vec2    m_targetPos;
	Sprite* m_targetSprite;
	float   m_coolDown;
	float   m_dashCoolDown;
};

class EntityEnemy : public Entity {
	virtual void run() = 0;
};

class EntityBullet : public Entity {
public:
	EntityBullet(vec2 pos, vec2 dir);
	virtual void run();
};