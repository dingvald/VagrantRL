#pragma once
#include "Zone.h"

class OverWorldMap
{
public:
	OverWorldMap();

	Zone& getZoneAt(sf::Vector2i world_coordinates);

private:
	std::vector< std::vector<Zone> > over_world_map;

	void populateZoneData();
};

