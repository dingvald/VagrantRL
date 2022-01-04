#pragma once
#include "Tile.h"
class Entity;

class Map
{
public:
	Map(unsigned int num_of_layers, unsigned int width, unsigned int height);

	std::list<Entity*>* getEntitiesAt(unsigned int layer, sf::Vector2i position);
	void placeEntity(Entity* entity, unsigned int layer, sf::Vector2i position);
	void removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position);

	void placeTile(Tile* tile, sf::Vector2u position);
	Tile* getTile(sf::Vector2i position);

	unsigned int getWidth();
	unsigned int getHeight();

	static sf::Vector2i toGridPosition(sf::Vector2i position);

private:
	unsigned int width;
	unsigned int height;

	std::vector<std::vector< std::vector< std::list<Entity*> > > > entitiesAt;
	std::vector< std::vector< std::unique_ptr<Tile> > > tiles;
};

