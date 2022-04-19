#pragma once
#include "System.h"
class CameraSystem : public System
{
public:
	void init() override;
	void update(const float dt) override;

private:
	Entity* following;
	unsigned int threshold = { 8 };
	unsigned int height;
	unsigned int width;
	bool is_origin_init = false;
	sf::Vector2f origin = { 0,0 };
	sf::Vector2f old_origin;

	void focusCamera(const float dt);
	void updateWorldPosition();
	sf::Vector2f lerpToTarget(sf::Vector2f pos, const sf::Vector2f target_pos, const float dt);
	
};

