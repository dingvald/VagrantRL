#pragma once

class Entity;

class MapChunk
{
public:
	MapChunk(sf::Vector2i world_coordinate, int chunk_size);
	sf::Vector2i world_coordinate;
	int size;

	std::list<Entity*>* at(int layer, sf::Vector2i tile_position);
private:
	std::vector< std::vector<std::vector< std::list<Entity*> > > > entities; // layer, x , y
};

