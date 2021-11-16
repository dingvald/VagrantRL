#pragma once

//Forward declarations

class TileMap;

//

class Renderer
{
private:
	TileMap *tilemap;

	void init();
	void end();
public:

	Renderer();
	~Renderer();

	void render(sf::RenderTarget* target);
};

