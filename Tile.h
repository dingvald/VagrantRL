#pragma once
#include "pch.h"

struct Tile
{
	unsigned int sprite;
	sf::Color fg;
	sf::Color bg;

	Tile(unsigned int sprite, sf::Color fg, sf::Color bg) : sprite(sprite), fg(fg), bg(bg) {}
};