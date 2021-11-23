#pragma once

struct Glyph;

class GlyphLayer : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

public:
	bool load(sf::Vector2u tileSize, std::vector<std::unique_ptr<Glyph> > *glyphs);
	bool loadTexture(const std::string& spriteSheet);

};

