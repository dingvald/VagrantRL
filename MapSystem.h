#pragma once
#include "System.h"
#include "Map.h"

class MapChunk;


class MapSystem : public System
{
public:
	void init() override;
	void update(const float dt) override;

private:
	std::unique_ptr<Map> map;
	sf::Vector2i num_of_loaded_chunks = { 3,3 };
	sf::Vector2i starting_position = { 1000,1000 };
	std::map<std::pair<int, int>, std::string> chunk_status;
	std::map<std::pair<int, int>, std::shared_ptr<MapChunk>> chunk_buffer;
	std::list<std::pair<int, int>> loaded_chunk_coords;
	sf::Vector2i world_position = { 0,0 };
	

	void buildInitialMap(sf::Vector2i starting_pos); // 9 map chunks loaded at all times (except on edges)

	void updateLoadedChunks();

	void printChunkStatus();

	void printLoadedChunkGrid();

	void printChunkBuffer();

	
};

