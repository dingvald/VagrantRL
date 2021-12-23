#pragma once
#include "pch.h"
#include "Tile.h"
#include "Globals.h"


class TileMap : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_tileset;
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

public:
	bool loadTexture(const std::string& spriteSheet)
	{
		if (!m_tileset.loadFromFile("tilesets\\" + spriteSheet))
		{
			return false;
		}
		return true;
	}

	bool load(sf::Vector2u tileSize, const Tile* tiles[gl::VIEWPORT_WIDTH][gl::VIEWPORT_HEIGHT], unsigned int width, unsigned int height)
	{
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// Populate the vertex array

		for (unsigned int i = 0; i < width; ++i)
		{
			for (unsigned int j = 0; j < height; ++j)
			{
				unsigned int tileNumber = tiles[i][j]->sprite;
				sf::Color col = tiles[i][j]->fg;

				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

				quad[0].color = col;
				quad[1].color = col;
				quad[2].color = col;
				quad[3].color = col;
				;
			}
		}

		return true;
	}
};

