#pragma once
#include "pch.h"
#include "Globals.h"
#include "Map.h"

class FOV
{
private:

	int radius = 5, origin_x = 0, origin_y = 0;
	Map* map;

public:

	FOV(Map * map, int radius) : map(map), radius(radius) {};

	void update(int org_x, int org_y)
	{
		origin_x = org_x;
		origin_y = org_y;

		rotateRay();
	}

	void rotateRay()
	{
		float x, y;
		int i;

		map->clearVisibility();

		for (i = 0; i < 360; ++i)
		{
			x = cos((float)i * 0.01745f);
			y = sin((float)i * 0.01745f);

			castRay(x, y);
		}
	}

	void castRay(float x, float y)
	{
		int i;
		float ox, oy;
		ox = (float)origin_x + 0.5f;
		oy = (float)origin_y + 0.5f;

		for (i = 0; i < radius; ++i)
		{
			if ((int)ox >= gl::SCENE_WIDTH || (int)ox < 0) return;
			if ((int)oy >= gl::SCENE_HEIGHT || (int)oy < 0) return;

			map->cell((int)ox, (int)oy)->isVisible = true;
			map->cell((int)ox, (int)oy)->isExplored = true;

			if (map->cell((int)ox, (int)oy)->blocksLight())
			{
				return;
			}

			

			ox += x;
			oy += y;
		}
	}

	void setRadius(int radius)
	{
		this->radius = radius;
	}
};