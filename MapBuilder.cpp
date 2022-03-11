#include "pch.h"
#include "Globals.h"
#include "MapBuilder.h"

std::shared_ptr<MapChunk> MapBuilder::build(sf::Vector2i coordinate)
{
    auto chunk_ptr = std::make_shared<MapChunk>(coordinate, gl::CHUNK_SIZE);

    // 

    return chunk_ptr;
}
