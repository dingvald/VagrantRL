#pragma once
#include "System.h"
#include "Map.h"

struct MapChunk
{

};

class MapSystem : public System
{
public:
	void init() override;

private:
	sf::Vector2u map_chunk_size = { 64, 64 };

	std::unique_ptr<Map> map;

	void buildInitialMap(); // 9 map chunk loaded at all times (except on edges)
	void loadMapChunk(sf::Vector2u worldcoords);

	static const unsigned int world_map_width = { 100 };
	static const unsigned int world_map_height = { 100 };

	unsigned int worldMap[world_map_width][world_map_height];
	sf::Vector2u current_position;
};

