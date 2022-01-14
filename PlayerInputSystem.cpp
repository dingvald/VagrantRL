#include "pch.h"
#include "PlayerInputSystem.h"

void PlayerInputSystem::init()
{
	signature.addComponentByType<PlayerAIComponent>();
	signature.addComponentByType<MyTurnComponent>();
}

void PlayerInputSystem::update(const float dt)
{
	if (registeredEntities.empty()) return;

	updateKeystates(dt);

	if (keyPressed(sf::Keyboard::Numpad8) || keyPressed(sf::Keyboard::Up))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(0, -1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad7))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(-1, -1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad4) || keyPressed(sf::Keyboard::Left))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(-1, 0)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad1))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(-1, 1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad2) || keyPressed(sf::Keyboard::Down))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(0, 1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad3))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(1, 1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad6) || keyPressed(sf::Keyboard::Right))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(1, 0)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad9))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(1, -1)).get());
	}
	else if (keyPressed(sf::Keyboard::Numpad5))
	{
		eventBus->publish(std::make_unique<MoveEvent>(registeredEntities.front(), sf::Vector2i(0, 0)).get());
	}
	else if (keyPressed(sf::Keyboard::Space))
	{
		world->save_player();
	}
}

bool PlayerInputSystem::keyPressed(unsigned int key)
{
	if (keystate[key].count > 0 && keystate[key].count < 4) return true;
	else return false;
}

void PlayerInputSystem::updateKeystates(const float dt)
{
	for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
	{
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key))
		{
			keystate[key].dt_count += dt;

			if (keystate[key].count == 0)
			{
				keystate[key].count++;
			}
			else if (keystate[key].count == 1 || keystate[key].count == 4)
			{
				if (keystate[key].dt_count >= first_press_delay)
				{
					keystate[key].count = 2;
					keystate[key].dt_count = 0.0f;
				}
				else
				{
					keystate[key].count = 4;
				}
			}
			else if (keystate[key].count == 2 || keystate[key].count == 5)
			{
				if (keystate[key].dt_count >= key_refractory_period)
				{
					keystate[key].count = 2;
					keystate[key].dt_count = 0.0f;
				}
				else
				{
					keystate[key].count = 5;
				}
			}
		}
		else
		{
			keystate[key].count = 0;
			keystate[key].dt_count = 0.0f;
		}
	}
}
