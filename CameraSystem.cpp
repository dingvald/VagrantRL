#include "pch.h"
#include "CameraSystem.h"

void CameraSystem::init()
{
	signature.addComponentByType<CameraFocusComponent>();

	height = gl::VIEWPORT_HEIGHT;
	width = gl::VIEWPORT_WIDTH;

	origin.x = static_cast<float>(gl::STARTING_COORDINATES.x * gl::CHUNK_SIZE * gl::TILE_SIZE);
	origin.y = static_cast<float>(gl::STARTING_COORDINATES.y * gl::CHUNK_SIZE * gl::TILE_SIZE);
}

void CameraSystem::update(const float dt)
{
	if (registeredEntities.size() == 1)
	{
		following = registeredEntities.front();
		focusCamera(dt);
	}
	else if (registeredEntities.size() > 1)
	{
		std::cout << "Error: too many entities for Camera to focus on!!!\n";
	}
	else
	{
		following = nullptr;
	}

	updateWorldPosition();
}

void CameraSystem::focusCamera(const float dt)
{
	if (following)
	{
		if (!is_origin_init)
		{
			origin.x = static_cast<float>(following->getComponent<PositionComponent>()->position.x) - ((static_cast<float>(width) / 2) * gl::TILE_SIZE);
			origin.y = static_cast<float>(following->getComponent<PositionComponent>()->position.y) - ((static_cast<float>(height) / 2) * gl::TILE_SIZE);
			world->cameraOrigin = origin;
			old_origin = origin;
			is_origin_init = true;
		}
		else
		{
			old_origin = origin;

			auto follow_pos = following->getComponent<PositionComponent>()->position;

			int tempx = follow_pos.x - ((width / 2) * gl::TILE_SIZE);
			int tempy = follow_pos.y - ((height / 2) * gl::TILE_SIZE);

			if (tempx < 0) tempx = 0;
			if (tempy < 0) tempy = 0;

			sf::Vector2f target = { static_cast<float>(tempx), static_cast<float>(tempy) };

			origin = lerpToTarget(origin, target, dt);
			world->cameraOrigin = origin;
		}
	}
}

void CameraSystem::updateWorldPosition()
{
	sf::Vector2i view_center;
	view_center.x = ((static_cast<int>(std::floorf(origin.x)) / gl::TILE_SIZE) + width / 2) / gl::CHUNK_SIZE;
	view_center.y = ((static_cast<int>(std::floorf(origin.y)) / gl::TILE_SIZE) + height / 2) / gl::CHUNK_SIZE;

	world->worldPosition = view_center;
}

sf::Vector2f CameraSystem::lerpToTarget(sf::Vector2f pos, const sf::Vector2f target_pos, const float dt)
{
	// ALERT MAGIC # 7!! used to scale dt for smooth scrolling

	sf::Vector2f delta = (pos - target_pos) * (dt*7);

	if (std::fabs(delta.x) < (dt*7))
	{
		delta.x = 0;
		pos.x = target_pos.x;
	}
	if (std::fabs(delta.y) < (dt*7))
	{
		delta.y = 0;
		pos.y = target_pos.y;
	}

	return pos - delta;
}


