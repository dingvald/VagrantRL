#pragma once

namespace gl
{
	const int WINDOW_WIDTH = 64; // width in tiles ***originally: 64
	const int WINDOW_HEIGHT = 40; // height in tiles ***originaly: 40
	const int VIEWPORT_WIDTH = WINDOW_WIDTH;
	const int VIEWPORT_HEIGHT = WINDOW_HEIGHT - 8; // 8 is for GUI window
	const int TILE_SIZE = 16;
	const int CHUNK_SIZE = 64; // Number of tiles per chunk
	const int ZONE_SIZE = 3; // Dimensions of zone -- each zone has (ZONE_SIZE)^2 chunks
	const int OVERWORLD_SIZE = 60; // Dimensions of OverWorldMap -- Over world map has (OVERWORLD_SIZE)^2 zones

	const sf::Vector2i STARTING_COORDINATES = { 50,50 };

	const unsigned int COMPONENT_MAX = 256;

	enum class Layer
	{
		Tile,
		Item,
		Actor,
		Effect,
		Total
	};
}