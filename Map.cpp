#include "pch.h"
#include "Map.h"
#include "Entity.h"
#include "SpatialConversions.h"


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

std::list<Entity*>* Map::getEntitiesAt(unsigned int layer, sf::Vector2i absolute_position)
{
	if (absolute_position.x < 0 || absolute_position.y < 0)
	{
		return nullptr;
	}

	// index into world map
	auto world_position = toWorldPosition(absolute_position);

	// index into tiles
	auto local_tile_position = toLocalTilePosition(absolute_position);

	// index into chunks in memory

	int chunk_index_x = (world_position.x - world->worldPosition.x) + 1;
	if (world->worldPosition.x == 0) chunk_index_x -= 1;
	int chunk_index_y = (world_position.y - world->worldPosition.y) + 1;
	if (world->worldPosition.y == 0) chunk_index_y -= 1;
	

	if (chunk_index_x >= chunk_load_width) chunk_index_x -= chunk_load_width;
	if (chunk_index_y >= chunk_load_width) chunk_index_y -= chunk_load_width;
	if (chunk_index_x < 0) chunk_index_x += chunk_load_width;
	if (chunk_index_y < 0) chunk_index_y += chunk_load_width;

	auto chunk_ptr = getChunk({ chunk_index_x, chunk_index_y });
	

	return chunk_ptr->at(layer, local_tile_position);
}

void Map::applyFuncToEntitiesInRect(unsigned int x_start, unsigned int y_start, unsigned int rect_width, unsigned int rect_height,
									std::function<void(Entity*)> fun)
{
	auto start_tile = toTilePosition({ static_cast<int>(x_start), static_cast<int>(y_start) });

	for (unsigned int layer = 0; layer < (unsigned int)gl::Layer::Total; ++layer)
	{
		for (int x = start_tile.x; x <= start_tile.x + (int)rect_width; ++x)
		{
			for (int y = start_tile.y; y <= start_tile.y + (int)rect_height; ++y)
			{
				auto entity_list = getEntitiesAt(layer, { x*gl::TILE_SIZE, y*gl::TILE_SIZE });
				if (!entity_list) continue;
				if (entity_list->empty()) continue;

				for (auto entity : *entity_list)
				{
					//apply the function
					fun(entity);
				}
			}
		}
	}
}

void Map::placeEntity(Entity* entity, unsigned int layer, sf::Vector2i absolute_position)
{
	auto list_ptr = getEntitiesAt(layer, absolute_position);
	if (!list_ptr)
	{
		std::cout << "Cannot place entity--List does not exist!\n";
	}
	else
	{
		list_ptr->push_back(entity);
		world->eventBus.publish(std::make_unique<EntityPlacedEvent>(entity).get());
	}
}

void Map::removeEntity(Entity* entity, unsigned int layer, sf::Vector2i absolute_position)
{
	auto list_ptr = getEntitiesAt(layer, absolute_position);
	if (!list_ptr)
	{
		std::cout << "Cannot remove entity--List does not exist...!\n";
	}
	else
	{
		list_ptr->remove(entity);
		world->eventBus.publish(std::make_unique<EntityRemovedEvent>(entity).get());
	}
}

void Map::addChunkToGrid(MapChunk* chunk)
{
	if (!chunk) return;

	sf::Vector2i grid_pos;
	grid_pos.x = chunk->world_coordinate.x - world->worldPosition.x + 1;
	grid_pos.y = chunk->world_coordinate.y - world->worldPosition.y + 1;

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
	int xL, yT;

	auto top_left_chunk = getChunk({ 0,0 });

	xL = top_left_chunk->world_coordinate.x * chunk_size * gl::TILE_SIZE;
	yT = top_left_chunk->world_coordinate.y * chunk_size * gl::TILE_SIZE;
	
	sf::Rect<int> ret(xL,yT,width,height);
	return ret;
}
