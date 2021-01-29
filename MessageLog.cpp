#include "pch.h"
#include "Globals.h"
#include "EventBus.h"
#include "MessageLog.h"

void MessageLog::render(sf::RenderTarget* target)
{
	if (!log.empty())
	{
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(16);

		for (int i = 0; i < log.size(); ++i)
		{
			std::string temp_str = "- " + log[(log.size() - 1) - i].text;

			sf::Color temp_col = log[(log.size() - 1) - i].color;
			temp_col.a -= 50 * ((log.size() - 1) - i);

			text.setPosition(origin.x * gl::TILE_SIZE, origin.y * gl::TILE_SIZE + message_offset * i);
			text.setString(temp_str);
			text.setFillColor(temp_col);

			target->draw(text);
		}
	}
}