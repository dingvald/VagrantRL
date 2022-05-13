#pragma once
#include "System.h"
class ViewportSystem : public System
{
public:
	void init() override;
	void update(const float dt) override;

private:

	void updateViewRect();

	bool viewportMoved();
	sf::Vector2i getMovementDirection();

	void updateOnScreenStatus(Entity* entity);
	void updateBoundaryEntities(sf::Vector2i direction);
	bool entityInViewport(Entity* entity);

	void takeOnScreenSnapshot();

	void onEntityPlacedEvent(EntityPlacedEvent* ev);
	void onEntityRemovedEvent(EntityRemovedEvent* ev);

	int viewport_buffer = 1;
	sf::Rect<float> viewport_rectangle{ static_cast<float>(0-(viewport_buffer*gl::TILE_SIZE)),
										static_cast<float>(0-(viewport_buffer*gl::TILE_SIZE)), 
										static_cast<float>((gl::VIEWPORT_WIDTH+viewport_buffer)*gl::TILE_SIZE), 
										static_cast<float>((gl::VIEWPORT_HEIGHT+viewport_buffer) * gl::TILE_SIZE) };
	bool update_init = false;
	sf::Vector2i old_origin;
};

