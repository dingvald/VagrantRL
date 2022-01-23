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
	sf::Vector2f origin = { 1.00f, 1.00f };
	sf::Vector2f old_origin;
	float updaterate = 30.0;
	float dt_count = 0.0;

	void focusViewport();
	bool viewportMoved();
	bool inViewport(sf::Vector2i pos);
	sf::Vector2f lerpToTarget(sf::Vector2f pos, const sf::Vector2f target_pos);
	
};

