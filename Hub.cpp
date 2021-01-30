#include "pch.h"
#include "Hub.h"
#include "EntityHandle.h"

namespace ecs {

// private:
void Hub::updateEntityMask(Entity const& entity, ComponentMask oldMask)
{
	ComponentMask newMask = entityMasks[entity];

	for (auto& system : systems)
	{
		ComponentMask systemSignature = system->getSignature();
		if (newMask.isNewMatch(oldMask, systemSignature))
		{
			system->registerEntity(entity);
		}
		else if (newMask.isNoLongerMatched(oldMask, systemSignature))
		{
			system->unRegisterEntity(entity);
		}
	}
}

// public:
Hub::Hub(std::unique_ptr<EntityManager> entityManager)
	: entityManager(std::move(entityManager))
{
}

void Hub::init(EventBus* eventBus)
{
	for (auto& system : systems)
	{
		system->eventBus = eventBus;
		system->init();
	}
}

void Hub::update(const float dt)
{
	// check if hub called init first
	for (auto& system : systems)
	{
		system->update(dt);
	}
}

void Hub::render(sf::RenderTarget* target)
{
	for (auto& system : systems)
	{
		system->render(target);
	}
}

EntityHandle Hub::createEntity(std::string name)
{
	return { entityManager->createEntity(name), this };
}

void Hub::addSystem(std::unique_ptr<System> system)
{
	system->registerHub(this);
	systems.push_back(std::move(system));
}

void Hub::destroyEntity(Entity entity)
{
	for (auto& system : systems)
	{
		system->unRegisterEntity(entity);
	}

	entityManager->destroy(entity);
}

} // namespace ecs
