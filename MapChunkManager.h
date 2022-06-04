#pragma once

#include "MapBuilder.h"

class World;
class MapChunk;

enum class ChunkState
{
	NONE,
	TO_BUILD,
	TO_SAVE,
	SAVED,
	TO_LOAD,
	LOADED,
	ACTIVE
};


class MapChunkManager
{
public:
	void init(World* world, sf::Vector2i world_position);
	void update(sf::Vector2i world_position);
	void cleanUp();
	void printChunkStatus(sf::Vector2i world_position);

private:
	World* _world;
	MapBuilder _map_builder;

	int _save_distance = 6;

	std::map< std::pair<int, int>, std::unique_ptr<MapChunk> > _chunk_map;
	std::map< std::pair<int, int>, ChunkState > _chunk_status;
	std::map<ChunkState, std::list<std::pair<int, int>>> _chunk_state_lists;

	std::list<std::pair<int, int>> _active_coordinates;
	std::list<std::pair<int, int>> _fuzzy_coordinates;

	std::deque< std::pair<int, int> > _build_queue;
	std::deque< std::pair<int, int> > _load_queue;
	std::deque< std::pair<int, int> > _save_queue;

	// Multithreading
	std::thread _process_in_queue;
	std::thread _process_out_queue;
	std::mutex _chunk_map_mutex;
	std::mutex _chunk_status_mutex;
	std::mutex _build_queue_mutex;
	std::mutex _load_queue_mutex;
	std::mutex _save_queue_mutex;
	bool _close_threads = false;

	void activateChunk(MapChunk* map_chunk);
	MapChunk* buildChunk(const sf::Vector2i world_position);
	MapChunk* loadChunk(const sf::Vector2i world_position);
	void saveChunk(const sf::Vector2i world_position);
	void setActiveChunks(const sf::Vector2i world_position);
	std::list<std::pair<int, int>> getCoordsAboutCenter(const sf::Vector2i world_position, int width);
	void th_buildOrLoadChunks();
	void th_saveChunks();
	void updateChunkStates(const sf::Vector2i world_position);

	//Chunk state manipulation for threads
	ChunkState getState(std::pair<int, int> coord); // thread safe
	void setState(std::pair<int, int> coord, ChunkState state); // thread safe
	
	std::map<ChunkState, std::string> status_enum_translator =
	{
		{ChunkState::NONE, "None"},
		{ChunkState::SAVED, "Saved"},
		{ChunkState::TO_BUILD, "To Build"},
		{ChunkState::LOADED, "Loaded"},
		{ChunkState::TO_SAVE, "To Save"},
		{ChunkState::ACTIVE, "Active"},
		{ChunkState::TO_LOAD, "To Load"}
	};
};

