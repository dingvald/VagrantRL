#pragma once
#include "System.h"

enum class Relationship
{
	inanimate,
	friendly,
	hostile
};

class CollisionSystem : public System
{
public:
	void init() override;

private:
	Relationship getRelationship(Entity* e1, Entity* e2);
	void onCollisionEvent(CollisionEvent* ev);

};

