#pragma once
#include "MapChunk.h"
#include "BuildPolicy.h"

class Entity;
class Component;

class MapBuilder
{
public:
	std::shared_ptr<MapChunk> build(sf::Vector2i coordinate);

	void init();
	void addBuildPolicy(std::string zone_type, BuildPolicy& policy);

private:
	std::map<std::string, BuildPolicy> build_policies_;
};

