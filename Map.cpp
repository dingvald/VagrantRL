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
	if (position.x < 0 || position.y < 0)
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

	int chunk_index_x = (worldmap_index_x - world->worldPosition.x) + 1;
	if (world->worldPosition.x == 0) chunk_index_x -= 1;
	int chunk_index_y = (worldmap_index_y - world->worldPosition.y) + 1;
	if (world->worldPosition.y == 0) chunk_index_y -= 1;
	

	if (chunk_index_x >= chunk_load_width) chunk_index_x -= chunk_load_width;
	if (chunk_index_y >= chunk_load_width) chunk_index_y -= chunk_load_width;
	if (chunk_index_x < 0) chunk_index_x += chunk_load_width;
	if (chunk_index_y < 0) chunk_index_y += chunk_load_width;

	auto chunk_ptr = getChunk({ chunk_index_x, chunk_index_y });
	

	return chunk_ptr->at(layer, { tile_index_x, tile_index_y });
}

void Map::applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height,
	std::function<void(Entity*)> fun)
{

	int x_start_tile = (int)x_start / gl::TILE_SIZE;
	int y_start_tile = (int)y_start / gl::TILE_SIZE;

	for (unsigned int layer = 0; layer < (unsigned int)gl::Layer::Total; ++layer)
	{
		for (int x = x_start_tile; x < x_start_tile + (int)rect_width; ++x)
		{
			for (int y = y_start_tile; y < y_start_tile + (int)rect_height; ++y)
			{
				//if (x > width || y > height) break;
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
		std::cout << "Cannot place entity--List does not exist...!\n";
	}
	else
	{
		list_ptr->push_back(entity);
	}
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i position)
{
	auto list_ptr = getEntitiesAt(layer, position);
	if (!list_ptr)
	{
		std::cout << "Cannot remove entity--List does not exist...!\n";
	}
	else
	{
		list_ptr->remove(entity);
	}
}

void Map::addChunkToGrid(MapChunk* chunk, sf::Vector2i grid_pos)
{
	if (!chunk) return;

	if (grid_pos.x < 0 || grid_pos.y < 0)
	{
		std::cout << "Grid position out of range\n";
		return;
	}
	if (grid_pos.x >= chunk_load_width || grid_pos.y >= chunk_load_width)
	{
		std::cout << "Grid position out of range\n";
		return;
	}

	map_chunk[grid_pos.x][grid_pos.y] = chunk;
	
}

MapChunk* Map::getChunk(sf::Vector2i grid_pos)
{
	if (grid_pos.x < 0 || grid_pos.y < 0) return nullptr;
	if (grid_pos.x >= chunk_load_width || grid_pos.y >= chunk_load_width) return nullptr;

	if (grid_pos.x < 0) grid_pos.x += chunk_load_width;
	if (grid_pos.x >= chunk_load_width) grid_pos.x -= chunk_load_width;

	if (grid_pos.y < 0) grid_pos.y += chunk_load_width;
	if (grid_pos.y >= chunk_load_width) grid_pos.y -= chunk_load_width;

	return map_chunk[grid_pos.x][grid_pos.y];
}

unsigned int Map::getWidth()
{
	return width;
}

unsigned int Map::getHeight()
{
	return height;
}

sf::Rect<int> Map::boundary()
{
	int xL, xR, yT, yB;

	auto top_left_chunk = getChunk({ 0,0 });

	xL = top_left_chunk->world_coordinate.x * chunk_size * gl::TILE_SIZE;
	yT = top_left_chunk->world_coordinate.y * chunk_size * gl::TILE_SIZE;
	
	sf::Rect<int> ret(xL,yT,width,height);
	return ret;
}
