#pragma once

#include "MapBuilder.h"

class World;
class MapChunk;

enum class ChunkStatus
{
	NONE,
	SAVED,
	FUZZY_IN,
	LOADED,
	FUZZY_OUT,
	ACTIVE
};


class MapChunkManager
{
public:
	void init(World* world, sf::Vector2i world_position);
	void update(sf::Vector2i world_position);
	void cleanUp();
	void printChunkStatus();

private:
	World* _world;
	MapBuilder _map_builder;
	std::map< std::pair<int, int>, std::unique_ptr<MapChunk> > _chunk_map;
	std::map< std::pair<int, int>, ChunkStatus > _chunk_status;
	std::deque< std::pair<int, int> > _build_queue;
	std::deque< std::pair<int, int> > _build_queue_buffer;
	std::thread _th_fill_chunk_map;
	std::mutex _chunk_map_mutex;
	std::mutex _chunk_status_mutex;
	std::mutex _build_queue_mutex;
	bool _close_buffer_thread = false;

	void activateChunk(MapChunk* map_chunk);
	MapChunk* buildChunk(sf::Vector2i world_position);
	MapChunk* loadChunk(sf::Vector2i world_position);
	void saveChunk(sf::Vector2i world_position);
	void setActiveChunks(sf::Vector2i world_position);
	std::list<std::pair<int, int>> getCoordsAboutCenter(sf::Vector2i world_position, int width);
	void updateBuildQueue(sf::Vector2i world_position);
	void fillChunkMap();
	
	std::map<ChunkStatus, std::string> status_enum_translator =
	{
		{ChunkStatus::NONE, "None"},
		{ChunkStatus::SAVED, "Saved"},
		{ChunkStatus::FUZZY_IN, "Fuzzy-in"},
		{ChunkStatus::LOADED, "Loaded"},
		{ChunkStatus::FUZZY_OUT, "Fuzzy-out"},
		{ChunkStatus::ACTIVE, "Active"}
	};
};

