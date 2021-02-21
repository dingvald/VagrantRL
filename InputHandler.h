#pragma once

struct KeyState
{
	bool pressed;
	unsigned int time_held = 0;
};

class InputHandler
{
private:
	KeyState keyStates[sf::Keyboard::KeyCount] = { false, 0 };
public:
	bool checkKey(sf::Keyboard::Key key);
	bool checkKeyHeld(sf::Keyboard::Key key);
	bool checkPressed(sf::Keyboard::Key key);
	bool checkReleased(sf::Keyboard::Key key);
	void checkReleasedAll(); // Used to reset all hasBeenPressed bools

	void update();
};