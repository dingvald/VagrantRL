#pragma once
#include "MapChunk.h"
#include "BuildPolicy.h"


class MapBuilder
{
public:
	std::unique_ptr<MapChunk> build(sf::Vector2i coordinate);

	void init();
	void addBuildPolicy(std::string zone_type, BuildPolicy& policy);

private:
	std::map<std::string, BuildPolicy> _build_policies;
};

