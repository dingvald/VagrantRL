#pragma once
#include "State.h"
#include "ECS.h"
#include "EntityManager.h"
#include "GUI.h"

class GameState : public State
{
private:
	// for ECS
	std::unique_ptr<ecs::EntityManager> entityManager;
	std::unique_ptr<ecs::Hub> ecsHub;
	std::unique_ptr<GUI> gui;

	// Player
	ecs::EntityHandle player;

	void init();
	void addSystems();
	void buildTileLayer(); // temporary until proper map class
public:
	GameState();
	virtual ~GameState();

	std::unique_ptr<EventBus> eventBus;

	// Functions

	void endState();
	void updateKeybinds(const float dt);
	void update(const float dt);
	void render(sf::RenderTarget *target);
};

