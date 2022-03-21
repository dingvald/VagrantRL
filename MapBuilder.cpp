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
