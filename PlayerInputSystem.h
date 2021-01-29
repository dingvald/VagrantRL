#pragma once
#include "ECS.h"
#include "InputHandler.h"

namespace ecs
{

class PlayerInputSystem : public System
{
private:
	InputHandler input;
public:
	PlayerInputSystem()
	{
		signature.addComponent<Player>();
		signature.addComponent<Motion>();
	}

	void init()
	{
	}

	void update(const float dt)
	{
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Motion> motion;
			
			parentHub->unpack(entity, motion);

			input.update();
			// Movement
#pragma region Movement Inputs
			if (input.checkPressed(sf::Keyboard::Key::Numpad6))
			{
				motion->dx = 1;
				motion->dy = 0;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad9))
			{
				motion->dx = 1;
				motion->dy = -1;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad8))
			{
				motion->dx = 0;
				motion->dy = -1;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad7))
			{
				motion->dx = -1;
				motion->dy = -1;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad4))
			{
				motion->dx = -1;
				motion->dy = 0;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad1))
			{
				motion->dx = -1;
				motion->dy = 1;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad2))
			{
				motion->dx = 0;
				motion->dy = 1;
			}
			else if (input.checkPressed(sf::Keyboard::Key::Numpad3))
			{
				motion->dx = 1;
				motion->dy = 1;
			}
			else
			{
				motion->dx = 0;
				motion->dy = 0;
			}
#pragma endregion


		}
	}
};

} // namespace ecs

