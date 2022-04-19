#pragma once
class MessageLog
{
public:
	void addMessage(std::string);
	void displayLog(sf::RenderTarget* target);

private:
	std::queue<std::string> log;
};

