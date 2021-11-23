#pragma once
#include "System.h"
#include "GlyphLayer.h"
#include "Glyph.h"

class RenderSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;
	void render(sf::RenderTarget* target) override;

	void addedEntity(Entity* entity) override;
	void removeEntity(Entity* entity) override;

private:
	std::map<Entity*, std::pair<Layer,unsigned int> > renderedEntities;
	std::vector< std::vector< std::unique_ptr<Glyph> > > glyphs;
	std::vector< std::unique_ptr<GlyphLayer> > glyphLayers;

	void createGlyph(Entity* entity);
	void changeGlyph(Entity* entity);
	void removeGlyph(Entity* entity);
};

