#pragma once
#include "System.h"
#include "GlyphLayer.h"
#include "Glyph.h"
#include "TileMap.h"

class RenderSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;
	void render(sf::RenderTarget* target) override;

	void addedEntity(Entity* entity) override;

private:
	std::map<Entity*, std::pair<gl::Layer,unsigned int> > renderedEntities;
	std::vector< std::vector< std::unique_ptr<Glyph> > > glyphs;
	std::vector< std::unique_ptr<GlyphLayer> > glyphLayers;
	TileMap tilemap;
	sf::Vector2f viewport_origin;
	float updaterate = 30.0;
	float dt_count = 0.0;
	

	void createGlyph(Entity* entity);
	void changeGlyph(Entity* entity);

	void updateEntities();
	void updateTilemap();

	void onViewportMoveEvent(ViewportMoveEvent* ev);

};

