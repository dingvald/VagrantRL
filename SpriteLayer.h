#pragma once

class Entity;
struct Sprite;

class SpriteLayer : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

public:
	SpriteLayer();

	bool loadSprite(const int tileSize, const Sprite& sprite);
	bool loadTexture(const std::string& spriteSheet);
	void clear();

};

