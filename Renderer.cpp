#include "pch.h"
#include "TileMap.h"
#include "Renderer.h"

void Renderer::init()
{
	tilemap = new TileMap();
}

void Renderer::end()
{
	if (tilemap) delete tilemap;
}

Renderer::Renderer()
{
	init();
}

Renderer::~Renderer()
{
	end();
}

void Renderer::render(sf::RenderTarget* target)
{
	target->draw(*tilemap);
}
