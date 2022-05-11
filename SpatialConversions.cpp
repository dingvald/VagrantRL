#include "pch.h"
#include "Globals.h"
#include "SpatialConversions.h"

sf::Vector2i toAbsolutePosition(const sf::Vector2i tile_position)
{
    return sf::Vector2i{tile_position.x * gl::TILE_SIZE, tile_position.y * gl::TILE_SIZE};
}

sf::Vector2i toTilePosition(const sf::Vector2i absolute_position)
{
    return sf::Vector2i{ absolute_position.x / gl::TILE_SIZE, absolute_position.y / gl::TILE_SIZE };
}

sf::Vector2i toLocalTilePosition(const sf::Vector2i absolute_position)
{
    auto world_position = toWorldPosition(absolute_position);
    auto tile_position = toTilePosition(absolute_position);
    int offset_x = world_position.x * gl::CHUNK_SIZE;
    int offset_y = world_position.y * gl::CHUNK_SIZE;

    return sf::Vector2i{ tile_position.x - offset_x, tile_position.y - offset_y };
}

sf::Vector2i toWorldPosition(const sf::Vector2i absolute_position)
{
    sf::Vector2i tpos = toTilePosition(absolute_position);

    return sf::Vector2i{tpos.x / gl::CHUNK_SIZE, tpos.y / gl::CHUNK_SIZE};
}
