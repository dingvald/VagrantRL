#include "pch.h"
#include "GameState.h"
#include "Globals.h"
#include "Engine.h"


// Init

void Engine::initWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(gl::WINDOW_WIDTH*gl::TILE_SIZE,
		gl::WINDOW_HEIGHT*gl::TILE_SIZE), "VagrantRL");	
}

void Engine::initStates()
{
	states.push(new GameState);
}

void Engine::closeEngine()
{
	std::cout << "Closing Window." << "\n";
	window->close();
}

// Constructor / Destructor

Engine::Engine()
{
	initWindow();
	initStates();
}

Engine::~Engine()
{
	while (!states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

// Functions

void Engine::updateDT()
{
	dt = dtClock.restart().asSeconds();
}

void Engine::updateSFMLEvents()
{
	while (window->pollEvent(sfEvent))
	{
		if (sfEvent.type == sf::Event::Closed)
		{
			closeEngine();
		}
	}
}

void Engine::update()
{
	updateSFMLEvents();

	if (!states.empty())
	{
		states.top()->update(dt);

		if (states.top()->getQuit())
		{
			states.top()->endState();
			delete states.top();
			states.pop();
		}
	}
	else
	{
		closeEngine();
	}
}

void Engine::render()
{
	window->clear();

	if (!states.empty())
	{
		states.top()->render(window);
	}

	window->display();
}

void Engine::run()
{
	while (window->isOpen())
	{
		updateDT();
		update();
		render();
	}
}
