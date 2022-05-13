#include "pch.h"
#include "ViewportSystem.h"

void ViewportSystem::init()
{
	signature.addComponentByType<OnScreenComponent>();
	signature.addComponentByType<PositionComponent>();

	eventBus->subscribe(this, &ViewportSystem::onEntityPlacedEvent);
	eventBus->subscribe(this, &ViewportSystem::onEntityRemovedEvent);

	updateViewRect();
}

void ViewportSystem::update(const float dt)
{
	updateViewRect();

	if (!update_init)
	{
		takeOnScreenSnapshot();
		update_init = true;
	}

	if (viewportMoved())
	{
		updateBoundaryEntities(getMovementDirection());
	}
}

void ViewportSystem::updateViewRect()
{
	old_origin = { static_cast<int>(viewport_rectangle.left), static_cast<int>(viewport_rectangle.top) };

	viewport_rectangle.left = world->cameraOrigin.x - viewport_buffer*gl::TILE_SIZE;
	viewport_rectangle.top = world->cameraOrigin.y - viewport_buffer*gl::TILE_SIZE;
}

bool ViewportSystem::viewportMoved()
{
	if (old_origin != sf::Vector2i{ static_cast<int>(viewport_rectangle.left), static_cast<int>(viewport_rectangle.top) })
	{
		return true;
	}
	return false;
}

sf::Vector2i ViewportSystem::getMovementDirection()
{
	sf::Vector2i current_origin{ static_cast<int>(viewport_rectangle.left), static_cast<int>(viewport_rectangle.top) };
	sf::Vector2i dir;

	// X
	if ((old_origin.x - current_origin.x) == 0)
	{
		dir.x = 0;
	}
	else if (old_origin.x - current_origin.x > 0)
	{
		dir.x = -1;
	}
	else
	{
		dir.x = 1;
	}

	// Y
	if ((old_origin.y - current_origin.y) == 0)
	{
		dir.y = 0;
	}
	else if (old_origin.y - current_origin.y > 0)
	{
		dir.y = -1;
	}
	else
	{
		dir.y = 1;
	}
	
	return dir;
}

void ViewportSystem::updateOnScreenStatus(Entity* entity)
{
	auto onScreen_comp = entity->getComponent<OnScreenComponent>();

	if (entityInViewport(entity) && !onScreen_comp)
	{
		entity->addComponent(new OnScreenComponent());
	}
	else if (!entityInViewport(entity) && onScreen_comp)
	{
		entity->removeComponent<OnScreenComponent>();
	}
}

void ViewportSystem::updateBoundaryEntities(sf::Vector2i direction)
{
	if (direction.x == 0 && direction.y == 0) return;

	auto fun = std::bind(&ViewportSystem::updateOnScreenStatus, this, std::placeholders::_1);

	// X
	if (direction.x == 1)
	{
		world->map->applyFuncToEntitiesInRect(	static_cast<unsigned int>(viewport_rectangle.left + viewport_rectangle.width),
												static_cast<unsigned int>(viewport_rectangle.top - gl::TILE_SIZE), 
												viewport_buffer, 
												gl::VIEWPORT_HEIGHT + 2,
												fun );
	}
	else if (direction.x == -1)
	{
		world->map->applyFuncToEntitiesInRect(	static_cast<unsigned int>(viewport_rectangle.left),
												static_cast<unsigned int>(viewport_rectangle.top - gl::TILE_SIZE), 
												viewport_buffer, 
												gl::VIEWPORT_HEIGHT + 2, 
												fun );
	}

	// Y
	if (direction.y == -1)
	{
		world->map->applyFuncToEntitiesInRect(	static_cast<unsigned int>(viewport_rectangle.left - gl::TILE_SIZE),
												static_cast<unsigned int>(viewport_rectangle.top), 
												gl::VIEWPORT_WIDTH + 2, 
												viewport_buffer, 
												fun );
	}
	else if (direction.y == 1)
	{
		world->map->applyFuncToEntitiesInRect(	static_cast<unsigned int>(viewport_rectangle.left - gl::TILE_SIZE),
												static_cast<unsigned int>(viewport_rectangle.top + viewport_rectangle.height), 
												gl::VIEWPORT_WIDTH + 2, 
												viewport_buffer, 
												fun );
	}
}

bool ViewportSystem::entityInViewport(Entity* entity)
{
	auto pos_comp = entity->getComponent<PositionComponent>();
	if (!pos_comp) return false;

	sf::Vector2f fpos{ static_cast<float>(pos_comp->position.x), static_cast<float>(pos_comp->position.y) };

	return viewport_rectangle.contains(fpos);
}

void ViewportSystem::takeOnScreenSnapshot()
{
	auto fun = std::bind(&ViewportSystem::updateOnScreenStatus, this, std::placeholders::_1);

	world->map->applyFuncToEntitiesInRect(	static_cast<unsigned int>(viewport_rectangle.left), 
											static_cast<unsigned int>(viewport_rectangle.top),
											gl::VIEWPORT_WIDTH, 
											gl::VIEWPORT_HEIGHT, 
											fun );
}

void ViewportSystem::onEntityPlacedEvent(EntityPlacedEvent* ev)
{
	updateOnScreenStatus(ev->entity);
}

void ViewportSystem::onEntityRemovedEvent(EntityRemovedEvent* ev)
{
	updateOnScreenStatus(ev->entity);
}
