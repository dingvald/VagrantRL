#include "pch.h"
#include "RenderSystem.h"

void RenderSystem::init()
{
	signature.addComponentByType<RenderComponent>();
	signature.addComponentByType<PositionComponent>();
}

void RenderSystem::update(const float dt)
{
	if (glyphs.size() != registeredEntities.size())
	{
		refreshGlyphs();
	}
}

void RenderSystem::refreshGlyphs()
{
	glyphs.clear();
	for (auto& entity : registeredEntities)
	{
		auto pos = entity->getComponent<PositionComponent>()->position;
		auto render = entity->getComponent<RenderComponent>();
		auto spr = render->sprite_id;
		auto col = render->color;

		glyphs.push_back(std::make_unique<Glyph>(spr, col, pos));
	}
}
