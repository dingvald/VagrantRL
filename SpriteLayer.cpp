#include "pch.h"
#include "Sprite.h"
#include "SpriteLayer.h"


void SpriteLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}

bool SpriteLayer::loadTexture(const std::string& spriteSheet)
{
	if (!m_tileset.loadFromFile("tilesets\\" + spriteSheet))
	{
		return false;
	}
	return true;
}

void SpriteLayer::clear()
{
	m_vertices = sf::VertexArray(sf::Quads);
}

SpriteLayer::SpriteLayer()
{
	m_vertices.setPrimitiveType(sf::Quads);
}

bool SpriteLayer::loadSprite(const int tileSize, const Sprite& sprite)
{
	int tileNumber = sprite.sprite_num;

	int tu = tileNumber % (m_tileset.getSize().x / tileSize);
	int tv = tileNumber / (m_tileset.getSize().x / tileSize);

	sf::Vertex quad[4];

	quad[0].position = sf::Vector2f(sprite.position.x, sprite.position.y);
	quad[1].position = sf::Vector2f(sprite.position.x + tileSize, sprite.position.y);
	quad[2].position = sf::Vector2f(sprite.position.x + tileSize, sprite.position.y + tileSize);
	quad[3].position = sf::Vector2f(sprite.position.x, sprite.position.y + tileSize);

	quad[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
	quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
	quad[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);

	quad[0].color = sprite.color;
	quad[1].color = sprite.color;
	quad[2].color = sprite.color;
	quad[3].color = sprite.color;

	for (int i = 0; i < 4; ++i) m_vertices.append(quad[i]);

	return true;
}
