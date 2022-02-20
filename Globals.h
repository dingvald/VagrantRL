#pragma once

namespace gl
{
	const int WINDOW_WIDTH = 64; // width in tiles
	const int WINDOW_HEIGHT = 40; // height in tiles
	const int VIEWPORT_WIDTH = 64;
	const int VIEWPORT_HEIGHT = 32;
	const int TILE_SIZE = 16;
	const int CHUNK_SIZE = 64;

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