#pragma once
#include "ECS.h"

namespace ecs
{

class CombatSystem : public System
{
private:

public:
	CombatSystem()
	{
		signature.addComponent<Attack>();
		signature.addComponent<Health>();
	}

	void init()
	{
		eventBus->subscribe(this, &CombatSystem::onAttackEvent);
	}

	void onAttackEvent(AttackEvent* attackEvent)
	{
		Entity attacker = attackEvent->attacker;
		Entity defender = attackEvent->defender;

		ComponentHandle<Health> defender_health;
		ComponentHandle<Attack> attacker_attack;

		parentHub->unpack(attacker, attacker_attack);
		parentHub->unpack(defender, defender_health);

		// calculate damage (just attack power for now)
		int damage = attacker_attack->attackPower;

		// print attack message to log
		std::stringstream sstream;
		sstream << parentHub->getEntityName(attacker) << " attacks the " << parentHub->getEntityName(defender) << " for " << attacker_attack->attackPower << " damage!";
		eventBus->publish(new LogEvent(sstream.str(), sf::Color::Red));


		// DEAL THE DAMAGE!
		eventBus->publish(new HealthEvent(defender, -(damage)));
	}
};

} // namespace ecs

