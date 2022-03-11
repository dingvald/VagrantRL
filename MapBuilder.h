#pragma once
#include "MapChunk.h"

class Entity;
class Component;

class MapBuilder
{
public:
	std::shared_ptr<MapChunk> build(sf::Vector2i coordinate);

private:

};

