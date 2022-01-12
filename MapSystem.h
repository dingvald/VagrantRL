#pragma once
#include "System.h"
#include "Map.h"



class MapSystem : public System
{
public:
	void init() override;

private:
	std::unique_ptr<Map> map;
	sf::Vector2u map_chunk_size = { 64, 64 };
	sf::Vector2u num_of_loaded_chunks = { 5,5 };

	static const unsigned int world_map_width = { 100 };
	static const unsigned int world_map_height = { 100 };

	unsigned int worldMap[world_map_width][world_map_height];
	sf::Vector2u current_position;

	void buildInitialMap(); // 25 map chunks loaded at all times (except on edges)

	void shiftActiveMap(sf::Vector2i dir);



	
};

