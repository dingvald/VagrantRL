#include "pch.h"

#include "RenderSystem.h"
#include "MovementSystem.h"
#include "InteractionSystem.h"
#include "CombatSystem.h"
#include "HealthSystem.h"
#include "TimeSystem.h"
#include "TurnMultiplexer.h"
#include "PlayerInputSystem.h"
#include "AIInputSystem.h"
#include "FOVSystem.h"
#include "ForestBuilder.h"
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
	player.addComponent(ecs::Faction(ecs::Faction::FRIENDLY));
	player.addComponent(ecs::Position(20,20, gl::Layer::BODIES));
	player.addComponent(ecs::Sprite(0, sf::Color(55,55,55)));
	player.addComponent(ecs::Time(100));
	player.addComponent(ecs::Vision(20));
	player.addComponent(ecs::Health(10));
	player.addComponent(ecs::Attack(3));
	ecsHub->addEntityToMap(player);

	
	/*
	ecs::EntityHandle test1;
	test1 = ecsHub->createEntity("Testfriend");
	test1.addComponent(ecs::AI(ecs::AI::BOT));
	test1.addComponent(ecs::Faction(ecs::Faction::FRIENDLY));
	test1.addComponent(ecs::Position(40, 5, gl::Layer::BODIES));
	test1.addComponent(ecs::Sprite(0, sf::Color::Blue));
	test1.addComponent(ecs::Time(100));
	test1.addComponent(ecs::Health(10));
	test1.addComponent(ecs::Attack(1));
	ecsHub->addEntityToMap(test1);
	*/



	// add test entities
	const int NUMBER_OF_TEST_ENTITIES = 10;

	ecs::EntityHandle test[NUMBER_OF_TEST_ENTITIES];

	for (int i = 0; i < NUMBER_OF_TEST_ENTITIES; ++i)
	{
		int px = rand() % gl::SCENE_WIDTH;
		int py = rand() % gl::SCENE_HEIGHT;

		test[i] = ecsHub->createEntity("Zombie");
		test[i].addComponent(ecs::AI(ecs::AI::MINDLESS));
		test[i].addComponent(ecs::Faction(ecs::Faction::ENEMY));
		test[i].addComponent(ecs::Position(px, py, gl::Layer::BODIES));
		test[i].addComponent(ecs::Sprite(0, sf::Color::Green));
		test[i].addComponent(ecs::Time(70));
		test[i].addComponent(ecs::Vision(10));
		test[i].addComponent(ecs::Health(10));
		test[i].addComponent(ecs::Attack(1));
		ecsHub->addEntityToMap(test[i]);
	}




	// build forest ** TEMPORARY **
	ForestBuilder forestBuilder(ecsHub.get());
	forestBuilder.setSparcity(5);
	forestBuilder.build();

	// Write welcome message in the Log
	eventBus->publish(new LogEvent("Welcome to Vagrant", sf::Color::White));
}

void GameState::addSystems()
{
	// add renderer
	auto sys_renderer = std::make_unique<ecs::RenderSystem>();
	ecsHub->addSystem(std::move(sys_renderer));

	// add time system
	auto sys_time = std::make_unique<ecs::TimeSystem>();
	ecsHub->addSystem(std::move(sys_time));

	// add the field of view system
	auto sys_fov = std::make_unique<ecs::FOVSystem>();
	ecsHub->addSystem(std::move(sys_fov));

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

	// add the turn multiplexer
	auto sys_turnMult = std::make_unique<ecs::TurnMultiplexer>();
	ecsHub->addSystem(std::move(sys_turnMult));

	// add the player input system
	auto sys_player_turn = std::make_unique<ecs::PlayerInputSystem>();
	ecsHub->addSystem(std::move(sys_player_turn));

	// add the bot input system
	auto sys_ai_turn = std::make_unique<ecs::AIInputSystem>();
	ecsHub->addSystem(std::move(sys_ai_turn));

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
			ecsHub->addEntityToMap(tile);
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
