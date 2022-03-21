#include "pch.h"
#include "Globals.h"
#include "OverWorldMap.h"

OverWorldMap::OverWorldMap()
{
	over_world_map.resize(gl::OVERWORLD_SIZE);
	for (int y = 0; y < gl::OVERWORLD_SIZE; ++y)
	{
		over_world_map[y].resize(gl::OVERWORLD_SIZE);
	}

	populateZoneData();
}

Zone& OverWorldMap::getZoneAt(sf::Vector2i world_coordinates)
{
	sf::Vector2i zone_coordinates;
	zone_coordinates.x = world_coordinates.x / gl::ZONE_SIZE;
	zone_coordinates.y = world_coordinates.y / gl::ZONE_SIZE;

	return over_world_map[zone_coordinates.x][zone_coordinates.y];
}

void OverWorldMap::populateZoneData()
{
	// get zone data from file;

}
