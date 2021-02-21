#pragma once
#include "pch.h"
#include "ECS.h"
#include "InputHandler.h"


namespace ecs
{

class PlayerInputSystem : public System
{
private:
	InputHandler inputHandler;

public:
	PlayerInputSystem()
	{

	}

	void init()
	{
		eventBus->subscribe(this, &PlayerInputSystem::onGetPlayerInputEvent);
	}

	void onGetPlayerInputEvent(GetPlayerInputEvent * player)
	{
		inputHandler.update();

		// Movement

		if (inputHandler.checkPressed(sf::Keyboard::Numpad6) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad6))
		{
			eventBus->publish(new MovementEvent(player->entity, 1, 0));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad9) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad9))
		{
			eventBus->publish(new MovementEvent(player->entity, 1, -1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad8) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad8))
		{
			eventBus->publish(new MovementEvent(player->entity, 0, -1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad7) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad7))
		{
			eventBus->publish(new MovementEvent(player->entity, -1, -1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad4) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad4))
		{
			eventBus->publish(new MovementEvent(player->entity, -1, 0));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad1) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad1))
		{
			eventBus->publish(new MovementEvent(player->entity, -1, 1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad2) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad2))
		{
			eventBus->publish(new MovementEvent(player->entity, 0, 1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad3) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad3))
		{
			eventBus->publish(new MovementEvent(player->entity, 1, 1));
		}
		else if (inputHandler.checkPressed(sf::Keyboard::Numpad5) || inputHandler.checkKeyHeld(sf::Keyboard::Numpad5))
		{
			eventBus->publish(new ActionEvent(100));
		}
		// end movement
	}

};

}// namespace ecs