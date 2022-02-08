#include "pch.h"
#include "Entity.h"
#include "Map.h"



Map::Map(unsigned int num_of_layers, int chunk_size, int chunk_load_width, World* world) 
	: chunk_size(chunk_size), chunk_load_width(chunk_load_width), world(world)
{
	width = chunk_size * chunk_load_width;
	height = width;

	map_chunk.resize(chunk_load_width);
	for (int x = 0; x < chunk_load_width; ++x)
	{
		map_chunk[x].resize(chunk_load_width);
		for (int y = 0; y < chunk_load_width; ++y)
		{
			map_chunk[x][y] = nullptr;
		}
	}
}

std::list<Entity*>* Map::getEntitiesAt(unsigned int layer, sf::Vector2i position)
{
	if ((position.x/gl::TILE_SIZE) > width - 1 || (position.y/gl::TILE_SIZE) > height - 1 || position.x < 0 || position.y < 0)
	{
		return nullptr;
	}
	// index into world map
	int worldmap_index_x = position.x / chunk_size / gl::TILE_SIZE;
	int worldmap_index_y = position.y / chunk_size / gl::TILE_SIZE;
	// index into tiles
	int tile_index_x = (position.x / gl::TILE_SIZE) - worldmap_index_x*chunk_size;
	int tile_index_y = (position.y / gl::TILE_SIZE) - worldmap_index_y*chunk_size;

	// index into chunks in memory

	int chunk_index_x = 0;
	int chunk_index_y = 0;

	if (world->worldPosition.x == 0)
	{
		chunk_index_x = column_index + (worldmap_index_x - world->worldPosition.x);
	}
	else
	{
		chunk_index_x = column_index + (worldmap_index_x - world->worldPosition.x + 1);
	}
	if (world->worldPosition.y == 0)
	{
		chunk_index_y = row_index + (worldmap_index_y - world->worldPosition.y);
	}
	else
	{
		chunk_index_y = row_index + (worldmap_index_y - world->worldPosition.y + 1);
	}

	if (chunk_index_x >= chunk_load_width) chunk_index_x -= chunk_load_width;
	if (chunk_index_y >= chunk_load_width) chunk_index_y -= chunk_load_width;
	

	if (worldmap_index_x == 0) chunk_index_x = 0;
	if (worldmap_index_y == 0) chunk_index_y = 0;

	return map_chunk[chunk_index_x][chunk_index_y]->at(layer, { tile_index_x, tile_index_y });
}

void Map::applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height,
	std::function<void(Entity*)> fun)
{

	int x_start_tile = x_start / gl::TILE_SIZE;
	int y_start_tile = y_start / gl::TILE_SIZE;

	for (unsigned int layer = 0; layer < (unsigned int)gl::Layer::Total; ++layer)
	{
		for (int x = x_start_tile; x < x_start_tile + rect_width; ++x)
		{
			for (int y = y_start_tile; y < y_start_tile + rect_height; ++y)
			{
				if (x > width || y > height) break;
				auto list = getEntitiesAt(layer, { x*gl::TILE_SIZE, y*gl::TILE_SIZE });
				if (!list) break;
				if (list->empty()) continue;

				for (auto e : *list)
				{
					fun(e);
				}
				
			}
		}
	}
}

void Map::placeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto list_ptr = getEntitiesAt(layer, position);
	if (!list_ptr)
	{
		std::cout << "List does not exist...!\n";
	}
	else
	{
		list_ptr->push_back(entity);
	}
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	getEntitiesAt(layer, position)->remove(entity);
}

void Map::addChunkToGrid(MapChunk* chunk)
{
	if (!chunk) return;

	auto chunk_position = chunk->world_coordinate;

	int chunk_index_x = chunk_position.x;
	int chunk_index_y = chunk_position.y;
	if (world->worldPosition.x != 0)
	{
		chunk_index_x = chunk_position.x - world->worldPosition.x + 1;
	}
	if (world->worldPosition.y != 0)
	{
		chunk_index_y = chunk_position.y - world->worldPosition.y + 1;
	}

	auto chunk_ptr = map_chunk[chunk_index_x][chunk_index_y].get();

	if (chunk_ptr)
	{
		//std::cout << "Chunk already exists at position " << chunk_index_x << ", " << chunk_index_x << "\n";
		delete chunk;
	}
	else
	{
		map_chunk[chunk_index_x][chunk_index_y].reset(chunk);
	}
}

MapChunk* Map::getChunk(sf::Vector2i relative_pos)
{
	if (relative_pos.x < 0 || relative_pos.y < 0) return nullptr;
	if (relative_pos.x > chunk_load_width || relative_pos.y > chunk_load_width) return nullptr;

	return map_chunk[relative_pos.x][relative_pos.y].get();
}

unsigned int Map::getWidth()
{
	return width;
}

unsigned int Map::getHeight()
{
	return height;
}

void Map::rotateMap(sf::Vector2i dir, int range)
{
	if (range > chunk_load_width)
	{
		std::cout << "ERROR: rotate range > than the loaded chunk number...\n";
		return;
	}

	column_index += dir.x * range;
	row_index += dir.y * range;

	if (column_index > chunk_load_width) column_index -= chunk_load_width;
	if (row_index > chunk_load_width) row_index -= chunk_load_width;
	if (column_index < 0) column_index += (chunk_load_width + column_index);
	if (row_index < 0) row_index += (chunk_load_width + row_index);

	std::cout << "Column index: " << column_index << "\n";
	std::cout << "Row index: " << row_index << "\n";
}