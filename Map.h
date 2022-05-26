#pragma once
#include "Tile.h"
#include "MapChunk.h"

class Entity;
class World;

class Map
{
public:
	Map(unsigned int num_of_layers, int chunk_size, int chunk_load_width, World *world);

	std::list<Entity*>* getEntitiesAt(unsigned int layer, sf::Vector2i absolute_position);
	void applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height, std::function<void(Entity*)> fun);
	void placeEntity(Entity* entity, unsigned int layer, sf::Vector2i absolute_position);
	void removeEntity(Entity* entity, unsigned int layer, sf::Vector2i absolute_position);

	void addChunkToGrid(MapChunk* chunk);
	MapChunk* getChunk(sf::Vector2i grid_pos);

	unsigned int getWidth();
	unsigned int getHeight();

	sf::Rect<int> boundary();

private:
	World* world;
	unsigned int width; // in tiles
	unsigned int height; // in tiles
	int chunk_size;
	int chunk_load_width;

	std::vector< std::vector< MapChunk* > > map_chunk; // [world_position.x][world_position.y]->at(layer,x,y)
};

