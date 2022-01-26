#pragma once
#include "Tile.h"
#include "Grid.h"
class Entity;
class World;

class Map
{
public:
	Map(unsigned int num_of_layers, unsigned int width, unsigned int height, World *world);

	std::list<Entity*>* getEntitiesAt(unsigned int layer, sf::Vector2i position);
	void applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height, std::function<void(Entity*)> fun);
	void placeEntity(Entity* entity, unsigned int layer, sf::Vector2i position);
	void removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position);

	void placeTile(Tile* tile, sf::Vector2u position);
	Tile* getTile(sf::Vector2i position);

	unsigned int getWidth();
	unsigned int getHeight();

	void rotateMap(sf::Vector2i dir, int range);

private:
	World* world;
	unsigned int width;
	unsigned int height;

	std::vector<Grid<std::list<Entity*>>> entities;
	std::vector< std::vector< std::unique_ptr<Tile> > > tiles;
};

