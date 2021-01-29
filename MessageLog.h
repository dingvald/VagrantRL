#pragma once

class EventBus;

struct Message
{
	std::string text;
	sf::Color color;
};

class MessageLog
{
private:
	EventBus* eventBus = nullptr;
	std::deque<Message> log;
	sf::Vector2u origin;
	unsigned int max_messages;
	unsigned int message_offset;
	sf::Font font;

public:
	MessageLog(unsigned int x, unsigned int y)
	{
		origin.x = x;
		origin.y = y;
		max_messages = 5;
		message_offset = 16;
		font.loadFromFile("fonts/terminus.ttf");
	}

	void init(EventBus* eventBus)
	{
		this->eventBus = eventBus;

		eventBus->subscribe(this, &MessageLog::onLogEvent);
	}

	void postMessage(Message message)
	{
		log.push_front(message);
		if (log.size() > max_messages)
		{
			log.pop_back();
		}
	}

	void onLogEvent(LogEvent* logEvent)
	{
		Message newMessage;
		newMessage.text = logEvent->message;
		newMessage.color = logEvent->color;

		postMessage(newMessage);
	}

	void render(sf::RenderTarget* target);

};
