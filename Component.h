#pragma once



// Forward declarations
class Entity;
class Event;

//

// Component Base Class //////////////////////////////////////////////////////////////////////////////////////

class Component
{
public:
	virtual ~Component() {};
	virtual void init() = 0;
	void remove();
	void setOwnerTo(Entity* entity);

protected:
	Entity* owner;
};


// Actual Component Classes //////////////////////////////////////////////////////////////////////////////////

class TestComponent : public Component
{
private:
	// Data
	std::string message;
	//
	void init();
public:
	TestComponent(std::string message);

	// Event handlers
	void onTestEvent(Event& ev);

};

class PositionComponent : public Component
{
private:
	// Data
	int x;
	int y;
	//
	void init();

public:
	PositionComponent(int, int);

	// Event Handlers

	void onTestEvent(Event& ev);
};