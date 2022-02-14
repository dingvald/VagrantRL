#include "pch.h"
#include "Globals.h"
#include "RenderSystem.h"

void RenderSystem::init()
{
	glyphs.resize((int)gl::Layer::Total);
	for (int l = 0; l < (int)gl::Layer::Total; ++l)
	{
		glyphLayers.push_back(std::make_unique<GlyphLayer>());
		glyphLayers.back()->loadTexture("simpleTileset.png");
	}

	tilemap.loadTexture("simpleTileset.png");
	

	signature.addComponentByType<RenderComponent>();
	signature.addComponentByType<PositionComponent>();

	eventBus->subscribe(this, &RenderSystem::onViewportMoveEvent);
}

void RenderSystem::update(const float dt)
{
	dt_count += dt;
	if (dt_count >= 1.000f / updaterate)
	{
		updateTilemap();
		updateEntities();
		dt_count = 0;
	}
}

void RenderSystem::render(sf::RenderTarget* target)
{
	target->draw(tilemap);
	for (int layer = 0; layer < (int)gl::Layer::Total; ++layer)
	{
		glyphLayers[layer]->load(sf::Vector2u(16, 16), &glyphs[layer]);
		target->draw(*glyphLayers[layer]);
	}

}

void RenderSystem::addedEntity(Entity* entity)
{
	createGlyph(entity);
}

void RenderSystem::createGlyph(Entity* entity)
{
	auto render = entity->getComponent<RenderComponent>();
	auto pos = entity->getComponent<PositionComponent>();

	auto layer = pos->layer;

	sf::Vector2f coordinatePosition{ (pos->position.x - viewport_origin.x),
		(pos->position.y - viewport_origin.y) };

	glyphs[static_cast<int>(layer)].push_back(std::make_unique<Glyph>(render->sprite_id, render->color, coordinatePosition));
}

void RenderSystem::changeGlyph(Entity* entity)
{
	createGlyph(entity);
}

void RenderSystem::updateEntities()
{
	sf::Vector2i org;

	for (int i = 1; i < (int)gl::Layer::Total; ++i)
	{
		glyphs[i].clear();
	}

	org.x = static_cast<int>(std::floorf(viewport_origin.x));
	org.y = static_cast<int>(std::floorf(viewport_origin.y));

	auto fun = std::bind(&RenderSystem::changeGlyph, this, std::placeholders::_1);

	world->map->applyFuncToEntitiesInRect(org.x, org.y, gl::VIEWPORT_WIDTH + 1, gl::VIEWPORT_HEIGHT + 1, fun);
}

void RenderSystem::updateTilemap()
{
	glyphs[(int)gl::Layer::Tile].clear();
	for (int x = viewport_origin.x/gl::TILE_SIZE; x < viewport_origin.x/gl::TILE_SIZE + (gl::VIEWPORT_WIDTH); ++x)
	{
		for (int y = viewport_origin.y/gl::TILE_SIZE; y < viewport_origin.y/gl::TILE_SIZE + (gl::VIEWPORT_HEIGHT); ++y)
		{
			sf::Vector2f coordinate_position = { x * gl::TILE_SIZE - viewport_origin.x, y * gl::TILE_SIZE - viewport_origin.y };

			if (x % 64 && y % 64)
			{
				glyphs[(int)gl::Layer::Tile].push_back(std::make_unique<Glyph>(2, sf::Color(100, 100, 100), coordinate_position));
			}
			else
			{
				glyphs[(int)gl::Layer::Tile].push_back(std::make_unique<Glyph>(2, sf::Color::Yellow, coordinate_position));
			}
		}
	}
}

void RenderSystem::onViewportMoveEvent(ViewportMoveEvent* ev)
{
	viewport_origin = ev->newOrigin;
}
