#include "pch.h"
#include "NPCInputSystem.h"

void NPCInputSystem::init()
{
	signature.addComponentByType<AIComponent>();
	signature.addComponentByType<MyTurnComponent>();
}

void NPCInputSystem::update(const float dt)
{
	if (registeredEntities.empty()) return;

	int rand_x, rand_y;

	rand_x = (rand() % 3) - 1;
	rand_y = (rand() % 3) - 1;

	eventBus->publish(new MoveEvent(registeredEntities.front(), { rand_x, rand_y }));
}
