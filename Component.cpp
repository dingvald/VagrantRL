#include "pch.h"
#include "Component.h"
#include "Entity.h"



void Component::setOwnerTo(Entity* entity)
{
	owner = entity;
}