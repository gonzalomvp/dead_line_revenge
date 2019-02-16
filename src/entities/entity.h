#pragma once

class  Component;
struct Message;

class Entity {
public:
	enum TType {
		EEnemyMelee,
		EEnemyBig,
		EEnemyRange,
		ETurret,
		EBoss,
		EPlayer,
		EPickup,
		EWeapon,
		EMine,
		EExplossion,
		ENuclearExplossion,
		EHUDMessage,
	};

	Entity(TType type) : m_type(type) {}
	~Entity();

	void  activate  ();
	void  deactivate();
	TType getType   () const { return m_type; }

	void run            (float deltaTime);
	void receiveMessage (Message* message);
	void addComponent   (Component* component);
private:
	TType                       m_type;
	std::vector<Component*> m_components;
};
