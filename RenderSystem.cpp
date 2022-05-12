#include "pch.h"
#include "Globals.h"
#include "Sprite.h"
#include "RenderSystem.h"

void RenderSystem::init()
{
	for (int l = 0; l < (int)gl::Layer::Total; ++l)
	{
		spriteLayers.push_back(std::make_unique<SpriteLayer>());
		spriteLayers.back()->loadTexture("simpleTileset.png");
	}

	tilemap.loadTexture("simpleTileset.png");
	

	signature.addComponentByType<RenderComponent>();
	signature.addComponentByType<PositionComponent>();
	signature.addComponentByType<OnScreenComponent>();
	
}

void RenderSystem::update(const float dt)
{
	viewport_origin = world->cameraOrigin;
	dt_count += dt;
	if (dt_count >= 1.000 / updaterate)
	{
		updateTilemap(); // inefficient
		updateEntities();
		dt_count = 0.0000;
	}
}

void RenderSystem::render(sf::RenderTarget* target)
{
	target->draw(tilemap);

	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		// draw all layers to the render target
		target->draw(*spriteLayers[layer]);
		spriteLayers[layer]->clear();
	}

}

Sprite RenderSystem::getEntitySprite(Entity* entity)
{
	auto render = entity->getComponent<RenderComponent>();
	auto pos = entity->getComponent<PositionComponent>();

	auto layer = pos->layer;

	sf::Vector2f screenPosition{(pos->position.x - viewport_origin.x),
								(pos->position.y - viewport_origin.y)};

	return Sprite(render->sprite_id, render->color, screenPosition);
}


void RenderSystem::updateEntities()
{
	for (auto& entity : registeredEntities)
	{
		auto layer = static_cast<int>(entity->getComponent<PositionComponent>()->layer);

		spriteLayers[layer]->loadSprite(gl::TILE_SIZE, getEntitySprite(entity));
	}
}

void RenderSystem::updateTilemap()
{
	
}