#include "pch.h"
#include "Glyph.h"
#include "GlyphLayer.h"

void GlyphLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}

bool GlyphLayer::load(const std::string& spriteSheet, sf::Vector2u tileSize, std::vector < std::unique_ptr<Glyph> > * glyphs)
{
	if (!m_tileset.loadFromFile("tilesets\\" + spriteSheet))
	{
		return false;
	}

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(glyphs->size() * 4);

	unsigned int count = 0;
	for (auto& glyph : *glyphs)
	{
		int tileNumber = glyph->sprite_num;

		int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
		int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

		sf::Vertex* quad = &m_vertices[count * 4];

		quad[0].position = sf::Vector2f(glyph->position.x, glyph->position.y);
		quad[1].position = sf::Vector2f(glyph->position.x + tileSize.x, glyph->position.y);
		quad[2].position = sf::Vector2f(glyph->position.x + tileSize.x, glyph->position.y + tileSize.y);
		quad[3].position = sf::Vector2f(glyph->position.x, glyph->position.y + tileSize.y);

		quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
		quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
		quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
		quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

		quad[0].color = glyph->color;
		quad[1].color = glyph->color;
		quad[2].color = glyph->color;
		quad[3].color = glyph->color;


		++count;
	}

	return false;
}
