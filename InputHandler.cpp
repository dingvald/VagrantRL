#include "pch.h"
#include "InputHandler.h"

bool InputHandler::checkKey(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool InputHandler::checkPressed(sf::Keyboard::Key key)
{
	if (checkKey(key) && !hasBeenPressed[key])
	{
		hasBeenPressed[key] = true;
		return true;
	}
	return false;
}

bool InputHandler::checkReleased(sf::Keyboard::Key key)
{
	if (hasBeenPressed[key])
	{
		if (!checkKey(key))
		{
			hasBeenPressed[key] = false;
			return true;
		}
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
