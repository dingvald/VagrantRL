#include "pch.h"
#include "ViewportSystem.h"

void ViewportSystem::init()
{
	signature.addComponentByType<ViewportFocusComponent>();

	height = gl::VIEWPORT_HEIGHT;
	width = gl::VIEWPORT_WIDTH;

	eventBus->subscribe(this, &ViewportSystem::onMoveEvent);
}

void ViewportSystem::update(const float dt)
{
	if (registeredEntities.size() == 1)
	{
		following = registeredEntities.front();
		centerViewport();
	}

	if (registeredEntities.size() > 1)
	{
		std::cout << "Error: too many entities for viewport to focus on!!!\n";
	}

	dt_count += dt;

	if (dt_count >= 1 / updaterate)
	{
		updateOnScreenEntities();
	}
}

void ViewportSystem::centerViewport()
{
	if (following)
	{
		old_origin = origin;

		auto follow_pos = following->getComponent<PositionComponent>()->position;

		int tempx = follow_pos.x - ( (width / 2) * gl::TILE_SIZE );
		int tempy = follow_pos.y - ( (height / 2) * gl::TILE_SIZE );

		// clamp viewport origin to map edges
		if (tempx < 0) tempx = 0;
		if (tempy < 0) tempy = 0;
		if (tempx > (world->currentMap->getWidth() - width) * gl::TILE_SIZE) tempx = (world->currentMap->getWidth() - width) * gl::TILE_SIZE;
		if (tempy > (world->currentMap->getHeight() - height) * gl::TILE_SIZE) tempy = (world->currentMap->getHeight() - height) * gl::TILE_SIZE;

		sf::Vector2f target = { static_cast<float>(tempx), static_cast<float>(tempy) };

		origin = lerpToTarget(origin, target);

		//std::cout << origin.x << ", " << origin.y << "\n";

		if (viewportMoved())
		{
			eventBus->publish(new ViewportMoveEvent(origin));
		}
	}
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

void ViewportSystem::updateOnScreenEntities()
{
	for (auto& e : on_screen_entities)
	{
		e->removeComponent<OnScreenComponent>();
	}
	on_screen_entities.clear();

	sf::Vector2i org;

	org.x = static_cast<int>(std::floorf(origin.x / gl::TILE_SIZE));
	org.y = static_cast<int>(std::floorf(origin.y / gl::TILE_SIZE));

	for (int layer = 1; layer < (int)gl::Layer::Total; ++layer)
	{
		for (int x = org.x; x < org.x + width + 1; ++x)
		{
			for (int y = org.y; y < org.y + height + 1; ++y)
			{
				auto listPtr = world->currentMap->getEntitiesAt(layer, { x,y });
				if (listPtr && !listPtr->empty())
				{
					auto ent = listPtr->front();
					on_screen_entities.push_back(ent);
					ent->addComponent(new OnScreenComponent());
				}
			}
		}
	}


}

void ViewportSystem::updateEntity(Entity* entity)
{
	auto pos_comp = entity->getComponent<PositionComponent>();
	auto render_comp = entity->getComponent<RenderComponent>();

	if (pos_comp && render_comp)
	{
		auto pos = pos_comp->position;

		if (inViewport(pos))
		{
			if (!entity->getComponent<OnScreenComponent>())
			{
				entity->addComponent(new OnScreenComponent);
				on_screen_entities.push_back(entity);
			}
		}
		else
		{
			if (entity->getComponent<OnScreenComponent>())
			{
				entity->removeComponent<OnScreenComponent>();
			}
		}
	}
}

void ViewportSystem::onMoveEvent(MoveEvent* ev)
{
	updateEntity(ev->entity);
}

sf::Vector2f ViewportSystem::lerpToTarget(sf::Vector2f pos, const sf::Vector2f target_pos)
{
	sf::Vector2f delta = (pos - target_pos) * 0.015f;

	if (std::fabs(delta.x) < 0.02)
	{
		delta.x = 0;
		pos.x = target_pos.x;
	}
	if (std::fabs(delta.y) < 0.02)
	{
		delta.y = 0;
		pos.y = target_pos.y;
	}

	return pos - delta;
}
