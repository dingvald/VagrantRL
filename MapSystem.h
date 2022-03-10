#pragma once
#include "System.h"
#include "Map.h"

class MapChunk;


class MapSystem : public System
{
public:
	void init() override;
	void cleanUp() override;
	void update(const float dt) override;

private:
	std::unique_ptr<Map> map;
	sf::Vector2i num_of_loaded_chunks = { 3,3 };
	sf::Vector2i starting_position = { 50,50 };
	std::map<std::pair<int, int>, std::shared_ptr<MapChunk>> chunk_buffer;
	std::list<std::pair<int, int>> active_chunk_coords;
	sf::Vector2i world_position = { 0,0 };
	sf::Vector2i old_world_position = { 0,0 };
	bool close_buffer_thread = false;
	std::thread fill_buffer_thread;
	std::mutex thread_control_lock;
	std::mutex chunk_buffer_lock;
	std::mutex build_queue_lock;
	std::deque<std::pair<int, int>> build_queue;
	std::deque<std::pair<int, int>> build_queue_buffer;

	void buildInitialMap(sf::Vector2i starting_pos); // 9 map chunks loaded at all times (except on edges)

	void updateLoadedChunks();

	std::list<std::pair<int, int>> getCoordsAboutCenter(sf::Vector2i center_pos, int width);

	void updateChunkBuildQ(sf::Vector2i old_center, sf::Vector2i new_center);

	void fillChunkBuffer();

	void printLoadedChunkGrid();

	void printChunkBuffer();

	
};

