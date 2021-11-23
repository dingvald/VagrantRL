#pragma once
#include "System.h"
#include "GlyphLayer.h"
#include "Glyph.h"

enum class RenderLayer
{
	Tiles,
	Items,
	Entities,
	Effects,
};

class RenderSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;
	void render(sf::RenderTarget* target) override;

private:
	std::map< RenderLayer, std::vector<std::unique_ptr<Glyph>> > glyphs;
};

