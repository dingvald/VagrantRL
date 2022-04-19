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
		registeredEntities.front()->addComponent(new MotionComponent({ 0,-1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad7))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ -1,-1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad4) || keyPressed(sf::Keyboard::Left))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ -1,0 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad1))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ -1,1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad2) || keyPressed(sf::Keyboard::Down))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ 0,1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad3))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ 1,1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad6) || keyPressed(sf::Keyboard::Right))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ 1,0 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad9))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ 1,-1 }));
	}
	else if (keyPressed(sf::Keyboard::Numpad5))
	{
		registeredEntities.front()->addComponent(new MotionComponent({ 0,0 }));
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
