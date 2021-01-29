#include "pch.h"
#include "State.h"

State::State()
{
	quit = false;
}

State::~State()
{
}

const bool& State::getQuit() const
{
	return quit;
}

void State::checkForQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		quit = true;
	}
}
