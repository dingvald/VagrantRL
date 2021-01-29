#include "pch.h"
#include "RenderSystem.h"


namespace ecs {

void RenderSystem::updateTileArray()
{
	clearTileArray();

	for (auto& entity : registeredEntities)
	{
		ComponentHandle<Position> position;
		ComponentHandle<Sprite> sprite;
		parentHub->unpack(entity, position, sprite);

		tileArray[position->layer][position->x + position->y * gl::SCENE_WIDTH] = new Glyph(sprite->spriteNum, sprite->moddedColor);
	}
}

void RenderSystem::clearTileArray()
{
	

	for (int layer = 0; layer < gl::TOTAL_LAYERS; ++layer)
	{
		int size = tileArray[layer].size();
		for (int i = 0; i < size; ++i)
		{
			if (tileArray[layer][i])
			{
				delete tileArray[layer][i];
			}
			tileArray[layer][i] = nullptr;
		}
	}
}

} // namespace ecs
