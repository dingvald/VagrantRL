#pragma once
#include "System.h"

struct keystatePair
{
	int count;
	float dt_count;
};

class PlayerInputSystem : public System
{
public:
	void init() override;

	void update(const float dt) override;
private:
	float key_refractory_period = { 1.0f / 60.0f };
	float first_press_delay = { 0.5 };
	keystatePair keystate[sf::Keyboard::KeyCount] = { 0, 0.0f };

	bool keyPressed(unsigned int key);
	void updateKeystates(const float dt);
};

