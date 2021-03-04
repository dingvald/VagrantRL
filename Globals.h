#pragma once

namespace gl
{
	const int WINDOW_WIDTH = 64; // width in tiles
	const int WINDOW_HEIGHT = 40; // height in tiles
	const int TILE_SIZE = 16;
	const int GUI_WIDTH = 64;
	const int GUI_HEIGHT = 8;
	const int SCENE_WIDTH = 64;
	const int SCENE_HEIGHT = WINDOW_HEIGHT - GUI_HEIGHT;

	enum Layer
	{
		TILES,
		ITEMS,
		BODIES,
		TOTAL_LAYERS
	};

	struct MapObject
	{
		MapObject(unsigned int id, unsigned int sprite, sf::Color color, bool blocks_light) : id(id), sprite(sprite), color(color), blocks_light(blocks_light) {};

		unsigned int id;
		unsigned int sprite;
		sf::Color color;
		bool blocks_light = false;
	};

	struct MapCell
	{
		std::list<MapObject*> objects[TOTAL_LAYERS];

		bool isVisible = false;
		bool isExplored = false;

		bool blocksLight()
		{
			if (!objects[gl::BODIES].empty())
			{
				if (objects[gl::BODIES].front()->blocks_light)
					return true;
				else
					return false;
			}
			return false;
		}
	};
}