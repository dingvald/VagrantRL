#include "pch.h"
#include "Globals.h"
#include "MapBuilder.h"

std::shared_ptr<MapChunk> MapBuilder::build(sf::Vector2i coordinate)
{
    auto chunk_ptr = std::make_shared<MapChunk>(coordinate, gl::CHUNK_SIZE);

    // Need to populate chunk with entities
    // Chunk is built and populated based off the zone's properties

    // get Zone properties
    



    return chunk_ptr;
}

void MapBuilder::init()
{
    // add all build policies
}

void MapBuilder::addBuildPolicy(std::string zone_type, BuildPolicy& policy)
{
    if (build_policies_.count(zone_type)) return;

    build_policies_.insert({ zone_type, policy });
}
