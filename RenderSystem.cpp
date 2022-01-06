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
	signature.addComponentByType<OnScreenComponent>();

	eventBus->subscribe(this, &RenderSystem::onViewportMoveEvent);
}

void RenderSystem::update(const float dt)
{
	dt_count += dt;
	if (dt_count >= 1 / updaterate)
	{
		updateTilemap();
		updateGlyphs();
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

void RenderSystem::removeEntity(Entity* entity)
{
	removeGlyph(entity);
}

void RenderSystem::createGlyph(Entity* entity)
{
	if (renderedEntities.count(entity)) return;

	auto render = entity->getComponent<RenderComponent>();
	auto pos = entity->getComponent<PositionComponent>();

	auto layer = pos->layer;

	auto index = glyphs[static_cast<int>(layer)].size();

	sf::Vector2f coordinatePosition{ (pos->position.x - viewport_origin.x),
		(pos->position.y - viewport_origin.y) };

	renderedEntities.insert(std::make_pair(entity, std::make_pair(layer, index)));
	glyphs[static_cast<int>(layer)].push_back(std::make_unique<Glyph>(render->sprite_id, render->color, coordinatePosition));
}

void RenderSystem::changeGlyph(Entity* entity)
{
	removeGlyph(entity);
	createGlyph(entity);
}

void RenderSystem::removeGlyph(Entity* entity)
{
	if (!renderedEntities.count(entity)) return;

	auto pair = renderedEntities.at(entity);
	auto layer = (unsigned int)pair.first;
	auto index = pair.second;

	auto last_index = glyphs[layer].size() - 1;

	Entity* last_entity;
	std::pair<gl::Layer, unsigned int> last_pair(pair.first, last_index);

	for (auto& e : renderedEntities)
	{
		if (e.second == last_pair)
		{
			last_entity = e.first;
			break;
		}
	}

	std::swap(glyphs[layer][index], glyphs[layer][last_index]);
	renderedEntities.at(last_entity).second = index;

	glyphs[layer].pop_back();
	renderedEntities.erase(entity);
}

void RenderSystem::updateGlyphs()
{
	for (auto& e : registeredEntities)
	{
		changeGlyph(e);
	}
}

void RenderSystem::updateTilemap()
{
	glyphs[(int)gl::Layer::Tile].clear();
	for (int x = viewport_origin.x/gl::TILE_SIZE; x < viewport_origin.x/gl::TILE_SIZE + (gl::VIEWPORT_WIDTH); ++x)
	{
		for (int y = viewport_origin.y/gl::TILE_SIZE; y < viewport_origin.y/gl::TILE_SIZE + (gl::VIEWPORT_HEIGHT); ++y)
		{
			sf::Vector2f coordinate_position = { x * gl::TILE_SIZE - viewport_origin.x, y * gl::TILE_SIZE - viewport_origin.y };

			Tile* tile = world->currentMap->getTile({ x,y });
			glyphs[(int)gl::Layer::Tile].push_back(std::make_unique<Glyph>(tile->sprite, tile->fg, coordinate_position));
		}
	}
}

void RenderSystem::onViewportMoveEvent(ViewportMoveEvent* ev)
{
	viewport_origin = ev->newOrigin;
}



