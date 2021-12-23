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
		if (viewportMoved())
		{
			updateOnScreenEntities();
		}
	}
}

void ViewportSystem::centerViewport()
{
	if (following)
	{
		auto follow_pos = following->getComponent<PositionComponent>()->position;

		old_origin.x = origin.x;
		old_origin.y = origin.y;


		origin.x = follow_pos.x - (width / 2);
		origin.y = follow_pos.y - (height / 2);

		if (origin.x < 0) origin.x = 0;
		if (origin.y < 0) origin.y = 0;
		if (origin.x > world->currentMap->getWidth() - width) origin.x = world->currentMap->getWidth() - width;
		if (origin.y > world->currentMap->getHeight() - height) origin.y = world->currentMap->getHeight() - height;

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
	if (pos.x < origin.x || pos.y < origin.y || pos.x > origin.x + width - 1 || pos.y > origin.y + height - 1)
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

	for (unsigned int layer = 0; layer < (unsigned int)gl::Layer::Total; ++layer)
	{ 
		for (int x = origin.x; x < origin.x + width; ++x)
		{
			for (int y = origin.y; y < origin.y + height; ++y)
			{
				std::list<Entity*>* listPtr = world->currentMap->getEntitiesAt(layer, { x,y });

				if (listPtr && !listPtr->empty())
				{
					on_screen_entities.push_back(listPtr->front());
					on_screen_entities.back()->addComponent(new OnScreenComponent);
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
