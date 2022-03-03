#include "pch.h"
#include "ViewportSystem.h"

void ViewportSystem::init()
{
	signature.addComponentByType<ViewportFocusComponent>();

	height = gl::VIEWPORT_HEIGHT;
	width = gl::VIEWPORT_WIDTH;
}

void ViewportSystem::update(const float dt)
{
	if (registeredEntities.size() == 1)
	{
		following = registeredEntities.front();
		focusViewport();
	}

	if (registeredEntities.size() > 1)
	{
		std::cout << "Error: too many entities for viewport to focus on!!!\n";
	}

	updateWorldPosition();
}

void ViewportSystem::focusViewport()
{
	if (following)
	{
		if (!is_origin_init)
		{
			origin.x = (float)following->getComponent<PositionComponent>()->position.x - ((width / 2) * gl::TILE_SIZE);
			origin.y = (float)following->getComponent<PositionComponent>()->position.y - ((height / 2) * gl::TILE_SIZE);
			world->viewportOrigin = origin;
			old_origin = origin;
			eventBus->publish(std::make_unique<ViewportMoveEvent>(origin, old_origin).get());

			is_origin_init = true;
		}
		else
		{
			old_origin = origin;

			auto follow_pos = following->getComponent<PositionComponent>()->position;

			int tempx = follow_pos.x - ((width / 2) * gl::TILE_SIZE);
			int tempy = follow_pos.y - ((height / 2) * gl::TILE_SIZE);

			// clamp viewport origin to map edges
			if (tempx < 0) tempx = 0;
			if (tempy < 0) tempy = 0;
			//if (tempx > (world->map->getWidth() - width) * gl::TILE_SIZE) tempx = (world->map->getWidth() - width) * gl::TILE_SIZE;
			//if (tempy > (world->map->getHeight() - height) * gl::TILE_SIZE) tempy = (world->map->getHeight() - height) * gl::TILE_SIZE;

			sf::Vector2f target = { static_cast<float>(tempx), static_cast<float>(tempy) };

			origin = lerpToTarget(origin, target);
			world->viewportOrigin = origin;

			if (viewportMoved())
			{
				eventBus->publish(std::make_unique<ViewportMoveEvent>(origin, old_origin).get());
			}
		}
	}
}

void ViewportSystem::updateWorldPosition()
{
	sf::Vector2i view_center;
	view_center.x = ((static_cast<int>(std::floorf(origin.x)) / gl::TILE_SIZE) + width / 2) / gl::CHUNK_SIZE;
	view_center.y = ((static_cast<int>(std::floorf(origin.y)) / gl::TILE_SIZE) + height / 2) / gl::CHUNK_SIZE;

	world->worldPosition = view_center;
}

bool ViewportSystem::viewportMoved()
{
	if (old_origin.x != origin.x || old_origin.y != origin.y)
	{
		return true;
	}

	return false;
}

bool ViewportSystem::inViewport(sf::Vector2i pos)
{
	if (pos.x < origin.x || pos.y < origin.y || pos.x > origin.x + (width - 1)*gl::TILE_SIZE || pos.y > origin.y + (height - 1)*gl::TILE_SIZE)
	{
		return false;
	}
	return true;
}

sf::Vector2f ViewportSystem::lerpToTarget(sf::Vector2f pos, const sf::Vector2f target_pos)
{
	sf::Vector2f delta = (pos - target_pos) * speed;

	if (std::fabs(delta.x) < speed)
	{
		delta.x = 0;
		pos.x = target_pos.x;
	}
	if (std::fabs(delta.y) < speed)
	{
		delta.y = 0;
		pos.y = target_pos.y;
	}

	return pos - delta;
}


