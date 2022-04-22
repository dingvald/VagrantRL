#pragma once
#include "System.h"

class GUISystem : public System
{
public:
	void init() override;
	void update(const float dt) override;
	void render(sf::RenderTarget* target) override;

private:
	sf::Vector2i origin = { 0, gl::WINDOW_HEIGHT - (gl::WINDOW_HEIGHT - gl::VIEWPORT_HEIGHT) };
	unsigned int height = gl::WINDOW_HEIGHT - gl::VIEWPORT_HEIGHT;
	unsigned int width = gl::WINDOW_WIDTH;

	sf::RectangleShape background;
	sf::Font font;
	sf::Text debug_text;
	std::list<std::string> debug_info;
	sf::Text fps;

	void setBackground(sf::Color col);
	void drawBackground(sf::RenderTarget* target);

	void getFPS(const float dt);
	void drawFPS(sf::RenderTarget* target);
	void setDebugInfo();
	void drawDebugInfo(sf::RenderTarget* target);

};

