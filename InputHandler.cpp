#include "pch.h"
#include "InputHandler.h"



bool InputHandler::checkKey(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool InputHandler::checkKeyHeld(sf::Keyboard::Key key)
{
	if (keyStates[key].time_held > 10)
		return true;
	else
		return false;
}

bool InputHandler::checkPressed(sf::Keyboard::Key key)
{
	if (checkKey(key) && !keyStates[key].pressed)
	{
		keyStates[key].pressed = true;
		return true;
	}
	return false;
}

bool InputHandler::checkReleased(sf::Keyboard::Key key)
{
	if (keyStates[key].pressed)
	{
		if (!checkKey(key))
		{
			keyStates[key].time_held = 0;
			keyStates[key].pressed = false;
			return true;
		}

		keyStates[key].time_held++;
		return false;
	}
	else
	{
		return false;
	}
}

void InputHandler::checkReleasedAll()
{
	for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		checkReleased((sf::Keyboard::Key)i);
	}
}

void InputHandler::update()
{
	checkReleasedAll();
}
