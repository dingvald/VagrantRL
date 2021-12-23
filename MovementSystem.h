#pragma once
#include "System.h"
class MovementSystem : public System
{
public:
	void init() override;
private:
	void move(Entity* entity, sf::Vector2i dir);
	Entity* checkForCollisionAt(sf::Vector2i coordinate);
	void onMoveEvent(MoveEvent* ev);
	void onSwapPlacesEvent(SwapPlacesEvent* ev);
};

