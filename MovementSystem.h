#pragma once
#include "Globals.h"
#include "System.h"

class MovementSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;

	void addedEntity(Entity* entity) override;
	void removeEntity(Entity* entity) override;

private:
	Entity* map[(unsigned int)Layer::Total][gl::WINDOW_WIDTH][gl::WINDOW_HEIGHT] = { nullptr };


};

