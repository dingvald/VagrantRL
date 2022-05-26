#pragma once

#include "MapBuilder.h"

class World;
class MapChunk;

enum class ChunkStatus
{
	NONE,
	SAVED,
	SAVING,
	LOADED,
	LOADING,
	ACTIVE
};

class MapChunkManager
{
public:
	MapChunkManager(World* world);
	void init(sf::Vector2i world_position);
	void update(sf::Vector2i world_position);
	void cleanUp();
	

private:
	World* _world;
	MapBuilder _map_builder;
	std::map< std::pair<int, int>, std::unique_ptr<MapChunk> > _chunk_map;
	std::map< std::pair<int, int>, ChunkStatus > _chunk_status;
	std::deque< std::pair<int, int> > _build_queue;
	std::deque< std::pair<int, int> > _build_queue_buffer;
	std::thread _fill_chunk_map;
	std::mutex _chunk_map_mutex;
	std::mutex _build_queue_mutex;

	std::list<std::pair<int, int>> getCoordsAboutCenter(sf::Vector2i world_position, int width);
	void updateBuildQueue(sf::Vector2i world_position);
	void fillChunkMap();

};

