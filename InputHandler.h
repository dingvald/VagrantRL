#pragma once

class InputHandler
{
private:
	bool hasBeenPressed[sf::Keyboard::KeyCount] = { false };
public:
	bool checkKey(sf::Keyboard::Key key);
	bool checkPressed(sf::Keyboard::Key key);
	bool checkReleased(sf::Keyboard::Key key);
	void checkReleasedAll(); // Used to reset all hasBeenPressed bools

	void update();
};