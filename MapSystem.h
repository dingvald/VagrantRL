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
	sf::Vector2i starting_position = { 1,1 }; // does not really "choose" position TODO: need to change how this affects viewport starting position...
	std::map<std::pair<int, int>, std::string> chunk_status;
	std::map<std::pair<int, int>, std::shared_ptr<MapChunk>> chunk_buffer;
	std::list<std::pair<int, int>> loaded_chunk_coords;
	

	void buildInitialMap(sf::Vector2i starting_pos); // 9 map chunks loaded at all times (except on edges)

	void shiftActiveMap(sf::Vector2i dir);

	void updateLoadedChunks();

	void loadOrBuildChunk(sf::Vector2i coords);

	void calculateWorldMapPosition(sf::Vector2f new_viewport_pos, sf::Vector2f old_viewport_pos);

	void onViewportMoveEvent(ViewportMoveEvent* ev);

	void printChunkStatus();

	void printLoadedChunkGrid();

	void printChunkBuffer();

	
};

