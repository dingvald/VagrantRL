#pragma once
#include "State.h"
#include "Renderer.h"
#include "World.h"



// Forward declarations


//

class GameState : public State
{
private:
	Renderer renderer;
	World world;

	void init();

public:
	GameState();
	virtual ~GameState();

	// Functions

	void endState();
	void updateKeybinds(const float dt);
	void update(const float dt);
	void render(sf::RenderTarget* target);
};

