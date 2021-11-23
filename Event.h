#pragma once

enum class EventType {
	nullEvent,
	testEvent,
	moveEvent,
	startTurnEvent,
	numOfEvents
};

class Event
{
public:

	// Data
	EventType typeID;
	std::unordered_map<std::string, std::string> parameters;

	// Functions
	// Create event and pass into fireEvent(Event) functions.
	Event(EventType, std::initializer_list < std::pair<const std::string, std::string> >);

};

class EventHandlerBase
{
private:
	virtual void call(Event& ev) = 0;

public:
	unsigned int priority;

	void execute(Event& ev)
	{
		call(ev);
	}
};

template<class T>
class EventHandler : public EventHandlerBase
{
public:
	typedef void (T::* MemberFunction)(Event&);

	EventHandler(T* instance, MemberFunction memberFunction)
		: instance(instance), memberFunction(memberFunction)
	{
	}
	void call(Event& ev)
	{
		(instance->*memberFunction)(ev);
	}

private:
	T* instance;
	MemberFunction memberFunction;

};

typedef std::map<std::type_index, std::unique_ptr<EventHandlerBase> > HandlerList;

class EventBus {
private:
	std::map<EventType, std::unique_ptr<HandlerList> > subscribers;
public:
	void publish(Event& ev)
	{
		if (subscribers.count(ev.typeID))
		{
			auto& handlers = subscribers.at(ev.typeID);

			if (handlers == nullptr)
			{
				return;
			}

			for (auto& handler : *handlers)
			{
				if (handler.second != nullptr)
				{
					handler.second->execute(ev);
				}
			}
		}
	}

	template<class T>
	void subscribe(T* instance, EventType typeID, void (T::*memberFunction)(Event&))
	{
		auto handlerPtr = std::unique_ptr<EventHandlerBase>(new EventHandler<T>(instance, memberFunction));
		std::pair<const std::type_index, std::unique_ptr<EventHandlerBase> > newEventPair = std::make_pair(std::type_index(typeid(T)), std::move(handlerPtr));

		if (subscribers.count(typeID))
		{
			subscribers.at(typeID)->insert(std::move(newEventPair));
		}
		else
		{
			auto newSubList = std::unique_ptr<HandlerList>(new HandlerList());

			subscribers.insert(std::make_pair(typeID, std::move(newSubList)));

			subscribers.at(typeID)->insert(std::move(newEventPair));
		}
	}

	template<class T>
	void unsubscribe(T* instance)
	{
		for (auto& subs : subscribers)
		{
			auto index = subs.second.get();

			if (index->count(std::type_index(typeid(*instance))))
			{
				index->erase(std::type_index(typeid(*instance)));
				continue;
			}
		}
	}

};
