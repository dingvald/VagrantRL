#pragma once
#include "pch.h"
#include "ECS.h"
#include "TileMap.h"
#include "Globals.h"

namespace ecs {

class RenderSystem : public System
{
private:
	sf::Texture* spriteSheet;

public:
	RenderSystem()
	{

	}

	// load sprite sheet and resize tile array
	void init()
	{
		spriteSheet = new sf::Texture();

		if (!spriteSheet->loadFromFile("tilesets/simpleTileset.png"))
			std::cout << "Cannot load the sprite sheet!!" << "\n";
	}

	// load and render the tile array
	void render(sf::RenderTarget* target)
	{
		TileMap tilemap;

		if (tilemap.load(*spriteSheet, sf::Vector2u(gl::TILE_SIZE, gl::TILE_SIZE), parentHub->getMap()->getCellArray(), gl::SCENE_WIDTH, gl::SCENE_HEIGHT))
		{
			target->draw(tilemap);
		}
	}

};

} // namespace ecs

