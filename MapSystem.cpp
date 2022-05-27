#include "pch.h"
#include "MapSystem.h"
#include "MapChunk.h"

void MapSystem::init()
{
	map = std::make_unique<Map>(static_cast<unsigned int>(gl::Layer::Total), gl::CHUNK_SIZE,
		3, world);

	world->map = map.get();

	world->worldPosition = starting_position;
	world_position = starting_position;
	old_world_position = starting_position;

	_map_chunk_manager.init(this->world, this->starting_position);
	buildInitialMap(starting_position);
	
}

void MapSystem::cleanUp()
{
	_map_chunk_manager.cleanUp();
}

void MapSystem::update(const float dt)
{
	if (world_position != world->worldPosition)
	{
		old_world_position = world_position;
		world_position = world->worldPosition;
		
		_map_chunk_manager.update(world_position);
	}
}

void MapSystem::buildInitialMap(sf::Vector2i starting_pos)
{
	std::cout << "Creating player...\n";

	auto player = std::make_unique<Entity>("Player");
	player->addComponent(new PositionComponent({starting_pos.x*gl::CHUNK_SIZE + 32,starting_pos.y*gl::CHUNK_SIZE + 32}, gl::Layer::Actor));
	player->addComponent(new RenderComponent(0, sf::Color(100, 100, 100)));
	player->addComponent(new TimeComponent(100));
	player->addComponent(new PlayerAIComponent());
	player->addComponent(new CameraFocusComponent()); 
	
	auto player_ptr = world->registerEntity(std::move(player));
	world->setAsPlayer(player_ptr);
}
