#pragma once
#include "System.h"
#include "SpriteLayer.h"
#include "Sprite.h"
#include "TileMap.h"

class RenderSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;
	void render(sf::RenderTarget* target) override;

private:
	std::vector< std::unique_ptr<SpriteLayer> > spriteLayers;
	TileMap tilemap;
	sf::Vector2f viewport_origin;
	double updaterate = 30.0;
	double dt_count = 0.0;
	

	Sprite getEntitySprite(Entity* entity);

	void updateEntities();
	void updateTilemap();

};

