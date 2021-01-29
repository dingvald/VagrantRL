#pragma once

class State
{
private:
	bool quit;

	virtual void init() = 0;
public:
	// Constructor / Destructor
	State();
	virtual ~State();

	// Functions

	const bool& getQuit() const;

	virtual void checkForQuit();

	virtual void endState() = 0;
	virtual void updateKeybinds(const float dt) = 0;
	virtual void update(const float dt) = 0;
	virtual void render(sf::RenderTarget * target) = 0;
};

