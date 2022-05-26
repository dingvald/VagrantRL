#pragma once
#include "System.h"
#include "Map.h"
#include "OverWorldMap.h"
#include "MapChunkManager.h"

class MapChunk;


class MapSystem : public System
{
public:
	void init() override;
	void cleanUp() override;
	void update(const float dt) override;

private:
	std::unique_ptr<Map> map;
	OverWorldMap overworldMap;
	MapChunkManager _map_chunk_manager;
	sf::Vector2i starting_position = gl::STARTING_COORDINATES;
	sf::Vector2i world_position = { 0,0 };
	sf::Vector2i old_world_position = { 0,0 };


	void buildInitialMap(sf::Vector2i starting_pos); // 9 map chunks loaded at all times (except on edges)

	
};

