#include "pch.h"
#include "Globals.h"
#include "MapBuilder.h"

std::unique_ptr<MapChunk> MapBuilder::build(sf::Vector2i coordinate)
{
    auto chunk_ptr = std::make_unique<MapChunk>(coordinate, gl::CHUNK_SIZE);

    // Need to populate chunk with entities
    // Chunk is built and populated based off the zone's properties

    // get Zone properties
    



    return std::move(chunk_ptr);
}

void MapBuilder::init()
{
    // add all build policies
}

void MapBuilder::addBuildPolicy(std::string zone_type, BuildPolicy& policy)
{
    if (_build_policies.count(zone_type)) return;

    _build_policies.insert({ zone_type, policy });
}
