#pragma once
#include "pch.h"

namespace sf
{
	template<class Archive>
	void serialize(Archive& ar, sf::Vector2i& vec)
	{
		ar(vec.x, vec.y);
	}

	template<class Archive>
	void serialize(Archive& ar, sf::Vector2f& vec)
	{
		ar(vec.x, vec.y);
	}

	template<class Archive>
	void serialize(Archive& ar, sf::Color& col)
	{
		ar(col.r, col.g, col.b);
	}
}