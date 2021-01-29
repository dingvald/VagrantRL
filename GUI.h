#pragma once
#include "EventBus.h"
#include "MessageLog.h"


/////////////////////////////////////////////////////////////////
// GUI
/////////////////////////////////////////////////////////////////

class GUI
{
private:
	EventBus* eventBus = nullptr;
	std::unique_ptr<MessageLog> log;
	sf::Vector2u origin;

public:
	GUI(unsigned int x, unsigned int y)
	{
		origin.x = x;
		origin.y = y;

		log = std::make_unique<MessageLog>(origin.x + 40, origin.y + 0);
	}

	void init(EventBus* eventBus)
	{
		this->eventBus = eventBus;
		log->init(eventBus);

		// subscribe to events
	}

	void update(const float dt);
	void render(sf::RenderTarget* target)
	{
		log->render(target);
	}
};
