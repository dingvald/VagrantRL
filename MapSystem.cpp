#include "pch.h"
#include "MapSystem.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), map_chunk_size.x * num_of_loaded_chunks.x,
		map_chunk_size.y * num_of_loaded_chunks.y);

	world->currentMap = map.get();

	buildInitialMap();
}

void MapSystem::buildInitialMap()
{
	
}

void MapSystem::shiftActiveMap(sf::Vector2i dir)
{
	// save row/column of tiles + entities depending on direction
	
	int x_start, x_end;
	int y_start, y_end;

	x_start = std::max(0, (int)(dir.x * (num_of_loaded_chunks.x - 1) * map_chunk_size.x));
	y_start = std::max(0, (int)(dir.y * (num_of_loaded_chunks.y - 1) * map_chunk_size.y));

	std::list<unsigned int> entity_ids;

	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		for (int x = x_start; x < x_start + map_chunk_size.x; ++x)
		{
			for (int y = y_start; y < y_start + map_chunk_size.y; ++y)
			{
				auto ent_list = map->getEntitiesAt(layer, { x,y });

				if (ent_list)
				{
					for (auto& e : *ent_list)
					{
						entity_ids.push_back(e->id);
					}
				}
			}
		}
	}
	

	world->save_entities(entity_ids);
}
