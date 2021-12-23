#pragma once
#include "pch.h"

struct Glyph
{
	unsigned int sprite_num;
	sf::Color color;
	sf::Vector2i position;

	Glyph(unsigned int sprite_num, sf::Color color, sf::Vector2i position)  
		: sprite_num(sprite_num), color(color), position(position) {}
};