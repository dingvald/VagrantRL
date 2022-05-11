#pragma once

sf::Vector2i toAbsolutePosition(const sf::Vector2i tile_position);
sf::Vector2i toTilePosition(const sf::Vector2i absolute_position);
sf::Vector2i toLocalTilePosition(const sf::Vector2i absolute_position);
sf::Vector2i toWorldPosition(const sf::Vector2i absolute_position);

