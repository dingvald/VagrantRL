#pragma once
#include "Globals.h"
#include "System.h"

class CoordinateSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;

	void addedEntity(Entity* entity) override;
	void removeEntity(Entity* entity) override;
};

