#include "pch.h"

#include "RenderSystem.h"
#include "MovementSystem.h"
#include "PlayerInputSystem.h"
#include "InteractionSystem.h"
#include "CombatSystem.h"
#include "HealthSystem.h"
#include "GameState.h"

void GameState::init()
{
	// create ecs members
	entityManager = std::make_unique<ecs::EntityManager>();
	ecsHub = std::make_unique<ecs::Hub>(std::move(entityManager));

	// create gui
	gui = std::make_unique<GUI>(0, gl::WINDOW_HEIGHT - gl::GUI_HEIGHT);

	// create event bus
	eventBus = std::make_unique<EventBus>();

	// add all systems
	addSystems();

	// initialize objects by passing the event bus
	ecsHub->init(eventBus.get());
	gui->init(eventBus.get());

	// build starting map ** TEMPORARY **
	buildTileLayer();

	// add player entity
	player = ecsHub->createEntity("Player");
	player.addComponent(ecs::Player());
	player.addComponent(ecs::Position(20,20, gl::Layer::BODIES));
	player.addComponent(ecs::Sprite(0, sf::Color::White));
	player.addComponent(ecs::Motion(0,0,1));
	player.addComponent(ecs::Health(10));
	player.addComponent(ecs::Attack(3));

	// add test entity
	ecs::EntityHandle test;
	test = ecsHub->createEntity("Testman");
	test.addComponent(ecs::Faction(ecs::Faction::ENEMY));
	test.addComponent(ecs::Position(24,20, gl::Layer::BODIES));
	test.addComponent(ecs::Sprite(0, sf::Color::Red));
	test.addComponent(ecs::Motion(0,0,1));
	test.addComponent(ecs::Health(10));
	test.addComponent(ecs::Attack(1));

	// add test tree entity
	ecs::EntityHandle tree;
	tree = ecsHub->createEntity("Tree");
	tree.addComponent(ecs::Position(30, 30, gl::Layer::BODIES));
	tree.addComponent(ecs::Sprite(5, sf::Color::Green));
	tree.addComponent(ecs::Motion(0, 0, 0));

	eventBus->publish(new LogEvent("Welcome to Vagrant", sf::Color::White));
}

void GameState::addSystems()
{
	// add player input
	auto sys_player_input = std::make_unique<ecs::PlayerInputSystem>();
	ecsHub->addSystem(std::move(sys_player_input));

	// add renderer
	auto sys_renderer = std::make_unique<ecs::RenderSystem>();
	ecsHub->addSystem(std::move(sys_renderer));

	// add movement
	auto sys_movement = std::make_unique<ecs::MovementSystem>();
	ecsHub->addSystem(std::move(sys_movement));

	// add interaction
	auto sys_interaction = std::make_unique<ecs::InteractionSystem>();
	ecsHub->addSystem(std::move(sys_interaction));

	// add combat system
	auto sys_combat = std::make_unique<ecs::CombatSystem>();
	ecsHub->addSystem(std::move(sys_combat));

	// add health system
	auto sys_health = std::make_unique<ecs::HealthSystem>();
	ecsHub->addSystem(std::move(sys_health));
}

void GameState::buildTileLayer()
{
	for (int i = 0; i < gl::SCENE_WIDTH; ++i)
	{
		for (int j = 0; j < gl::SCENE_HEIGHT; ++j)
		{
			ecs::EntityHandle tile;
			tile = ecsHub->createEntity("Tile");
			tile.addComponent(ecs::Position(i, j, gl::Layer::TILES));
			tile.addComponent(ecs::Sprite(2, sf::Color(25, 25, 25)));
		}
	}
}

GameState::GameState()
{
	init();
}

GameState::~GameState()
{
}

void GameState::endState()
{
	std::cout << "Ending Game State." << "\n";
}

void GameState::updateKeybinds(const float dt)
{
	checkForQuit();
}

void GameState::update(const float dt)
{
	updateKeybinds(dt);
	
	//update ecs
	ecsHub->update(dt);

	//update gui
	gui->update(dt);
}

void GameState::render(sf::RenderTarget* target)
{
	//render ecs
	ecsHub->render(target);

	//render gui
	gui->render(target);
}
