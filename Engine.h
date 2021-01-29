#pragma once

class State;

class Engine
{
private:
	// Variables
	sf::RenderWindow *window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	// Init
	void initWindow();
	void initStates();

	void closeEngine();
public:
	// Constructor / Destructor
	Engine();
	virtual ~Engine();

	// Functions
	void updateDT();
	void updateSFMLEvents();
	void update();
	void render();
	void run();
};

