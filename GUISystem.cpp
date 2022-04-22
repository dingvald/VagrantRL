#include "pch.h"
#include "GUISystem.h"

void GUISystem::init()
{
	if (!font.loadFromFile("fonts\\terminus.ttf"))
	{
		std::cout << "Could not load font!\n";
	}
	else
	{
		fps.setFont(font);
		fps.setCharacterSize(12);
		fps.setFillColor(sf::Color::Yellow);
		debug_text.setFont(font);
		debug_text.setCharacterSize(12);
		debug_text.setFillColor(sf::Color::Yellow);
	}
	

	setBackground(sf::Color(10, 10, 10));
}

void GUISystem::update(const float dt)
{
	setDebugInfo();
	getFPS(dt);
}

void GUISystem::render(sf::RenderTarget* target)
{
	drawBackground(target);
	drawDebugInfo(target);
	drawFPS(target);
}

void GUISystem::setBackground(sf::Color col)
{
	background.setSize({ (float)width*gl::TILE_SIZE, (float)height*gl::TILE_SIZE });
	background.setFillColor(col);
	background.setOutlineThickness(-1.0f);
	background.setOutlineColor(sf::Color(100, 100, 100));
	background.setPosition({ (float)origin.x * gl::TILE_SIZE, (float)origin.y * gl::TILE_SIZE });
}

void GUISystem::drawBackground(sf::RenderTarget* target)
{
	target->draw(background);
}

void GUISystem::getFPS(const float dt)
{
	int f = 1 / dt;
	std::stringstream ss;
	ss << "FPS: " << f;
	fps.setString(ss.str());
}

void GUISystem::drawFPS(sf::RenderTarget* target)
{
	fps.setPosition({ ((float)origin.x + 32) * gl::TILE_SIZE, ((float)origin.y) * gl::TILE_SIZE });

	target->draw(fps);
}

void GUISystem::setDebugInfo()
{
	debug_info.clear();
	debug_info.push_back("Debug Info:");
	// world map position
	std::stringstream ss;
	ss << "Position in World: [" << world->worldPosition.x << ", " << world->worldPosition.y << "]";
	debug_info.push_back(ss.str());

	ss.str(""); // clear string stream

	// player position on active map
	auto pos = world->player.ptr->getComponent<PositionComponent>()->position / gl::TILE_SIZE;

	ss << "Player position: [" << pos.x << ", " << pos.y << "]";
	debug_info.push_back(ss.str());

	//
}

void GUISystem::drawDebugInfo(sf::RenderTarget* target)
{
	int count = 0;
	for (auto d : debug_info)
	{
		debug_text.setString(d + "\n");
		debug_text.setPosition({ ((float)origin.x + 40) * gl::TILE_SIZE, ((float)origin.y + count) * gl::TILE_SIZE });

		target->draw(debug_text);

		++count;
	}
}
