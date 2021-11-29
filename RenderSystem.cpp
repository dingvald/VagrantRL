#include "pch.h"
#include "RenderSystem.h"

void RenderSystem::init()
{
	glyphs.resize((int)Layer::Total);
	for (int l = 0; l < (int)Layer::Total; ++l)
	{
		glyphLayers.push_back(std::make_unique<GlyphLayer>());
		glyphLayers.back()->loadTexture("simpleTileset.png");
	}

	signature.addComponentByType<RenderComponent>();
	signature.addComponentByType<PositionComponent>();
}

void RenderSystem::update(const float dt)
{
	
}

void RenderSystem::render(sf::RenderTarget* target)
{
	for (int l = (int)Layer::Tile; l < (int)Layer::Total; ++l)
	{
		glyphLayers[l]->load(sf::Vector2u(16, 16), &glyphs[l]);
		target->draw(*glyphLayers[l]);
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

	renderedEntities.insert(std::make_pair(entity, std::make_pair(layer, index)));
	glyphs[static_cast<int>(layer)].push_back(std::make_unique<Glyph>(render->sprite_id, render->color, pos->position));
}

void RenderSystem::changeGlyph(Entity* entity)
{
	removeGlyph(entity);
	createGlyph(entity);
}

void RenderSystem::removeGlyph(Entity* entity)
{
	auto pair = renderedEntities.at(entity);
	auto layer = (unsigned int)pair.first;
	auto index = pair.second;

	auto last_index = glyphs[layer].size() - 1;

	Entity* last_entity;
	std::pair<Layer, unsigned int> last_pair(pair.first, last_index);

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
