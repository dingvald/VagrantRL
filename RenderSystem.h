#pragma once
#include "ECS.h"
#include "TileMap.h"
#include "Globals.h"

namespace ecs {

class RenderSystem : public System
{
private:
	sf::Texture *spriteSheet;
	std::vector< std::vector<Glyph*> > tileArray;

public:
	RenderSystem()
	{
		signature.addComponent<Sprite>();
		signature.addComponent<Position>();
	}
	
	// load sprite sheet and resize tile array
	void init()
	{
		spriteSheet = new sf::Texture();

		if (!spriteSheet->loadFromFile("tilesets/simpleTileset.png"))
			std::cout << "Cannot load the sprite sheet!!" << "\n";

		tileArray.resize(gl::TOTAL_LAYERS, std::vector<Glyph*>(gl::SCENE_HEIGHT * gl::SCENE_WIDTH));
	}

	// load and render the tile array
	void render(sf::RenderTarget* target)
	{
		TileMap tilemap;

		updateTileArray();

		for (int layer = 0; layer < gl::TOTAL_LAYERS; ++layer)
		{
			if (tilemap.load(*spriteSheet, sf::Vector2u(gl::TILE_SIZE, gl::TILE_SIZE), tileArray[layer], gl::SCENE_WIDTH, gl::SCENE_HEIGHT))
			{
				target->draw(tilemap);
			}
		}
	}

	void updateTileArray(); // calls clearTileArray()
	void clearTileArray();
};

} // namespace ecs

