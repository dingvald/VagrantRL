#pragma once
#include "System.h"
class ViewportSystem : public System
{
public:
	void init() override;
	void update(const float dt) override;

private:
	Entity* following;
	unsigned int threshold = { 8 };
	unsigned int height;
	unsigned int width;
	sf::Vector2i origin = { -1, -1 };
	sf::Vector2i old_origin;
	std::list<Entity*> on_screen_entities;
	float updaterate = 30.0;
	float dt_count = 0.0;

	void centerViewport();
	bool viewportMoved();
	bool inViewport(sf::Vector2i pos);
	void updateOnScreenEntities();
	void updateEntity(Entity* entity);
	void onMoveEvent(MoveEvent* ev);
};

