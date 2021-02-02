#pragma once
#include "ComponentManager.h"
#include "Entity.h"

namespace ecs {

#pragma region Component Template
struct ComponentCounter
{
	static int familyCounter;
};

template <typename ComponentType>
struct Component
{
	static inline int family()
	{
		static int family = ComponentCounter::familyCounter++;
		return family;
	}
};

template <typename C>
static int GetComponentFamily()
{
	return Component<typename std::remove_const<C>::type>::family();
}
#pragma endregion 

/********************************************************
	COMPONENTS
*********************************************************/

// Information
struct Information : Component<Information>
{
	Information(std::string name) : name(name) {};

	std::string name;
};

// Position
struct Position : Component<Position>
{
	Position(int x, int y, unsigned int layer) : x(x), y(y), layer(layer) {};

	int x;
	int y;
	unsigned int layer;
};

// Motion
struct Motion : Component<Motion>
{
	Motion(int dx, int dy, int spd) : dx(dx), dy(dy), speed(spd) {};

	int dx;
	int dy;
	int speed;
};

// Sprite
struct Sprite : Component<Sprite>
{
	Sprite(unsigned int spriteNum, sf::Color col) : spriteNum(spriteNum),
		baseColor(col), moddedColor(col){};

	unsigned int spriteNum;
	sf::Color baseColor;
	sf::Color moddedColor;
};

// Health
struct Health : Component<Health>
{
	Health(int health, int maxHealth) : health(health), maxHealth(maxHealth) {};
	Health(int health) : health(health), maxHealth(health) {};

	int health;
	int maxHealth;
};

// Player
struct Player : Component<Player>
{
	// Empty for now
};

// AI
struct AI : Component<AI>
{
	// Empty for now
};

// Attack
struct Attack : Component<Attack>
{
	Attack(int attackPower) : attackPower(attackPower) {};
	int attackPower;
};

// Faction
struct Faction : Component<Faction>
{
	Faction(unsigned int faction) : faction(faction) {};

	unsigned int faction;

	enum Relationship
	{
		FRIENDLY,
		NEUTRAL,
		ENEMY	
	};
};


} // namespace ecs

