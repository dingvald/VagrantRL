#pragma once
#include "pch.h"

struct Sprite
{
	unsigned int sprite_num;
	sf::Color color;
	sf::Vector2f position;

	Sprite(unsigned int sprite_num, sf::Color color, sf::Vector2f position)  
		: sprite_num(sprite_num), color(color), position(position) {}
};