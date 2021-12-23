#pragma once
#include "Event.h"

class EventHandlerBase
{
private:
	virtual void call(Event* ev) = 0;

public:
	unsigned int priority;

	void execute(Event* ev)
	{
		call(ev);
	}
};

template<class T, class EventType>
class EventHandler : public EventHandlerBase
{
public:
	typedef void (T::* MemberFunction)(EventType*);

	EventHandler(T* instance, MemberFunction memberFunction)
		: instance(instance), memberFunction(memberFunction)
	{
	}
	void call(Event* ev)
	{
		(instance->*memberFunction)(static_cast<EventType*>(ev));
	}

private:
	T* instance;
	MemberFunction memberFunction;

};

typedef std::map<std::type_index, std::unique_ptr<EventHandlerBase> > HandlerList;

class EventBus {
private:
	std::map<std::type_index, std::unique_ptr<HandlerList> > subscribers;
public:

	template<typename EventType>
	void publish(EventType* ev)
	{
		if (subscribers.count(std::type_index(typeid(EventType))) )
		{
			auto& handlers = subscribers.at(std::type_index(typeid(EventType)));

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

	template<class T, class EventType>
	void subscribe(T* instance, void (T::*memberFunction)(EventType*))
	{
		auto handlerPtr = std::unique_ptr<EventHandlerBase>(new EventHandler<T,EventType>(instance, memberFunction));
		std::pair<const std::type_index, std::unique_ptr<EventHandlerBase> > newEventPair = std::make_pair(std::type_index(typeid(T)), std::move(handlerPtr));

		if (subscribers.count(std::type_index(typeid(EventType))))
		{
			subscribers.at(std::type_index(typeid(EventType)))->insert(std::move(newEventPair));
		}
		else
		{
			auto newSubList = std::unique_ptr<HandlerList>(new HandlerList());

			subscribers.insert(std::make_pair(std::type_index(typeid(EventType)), std::move(newSubList)));

			subscribers.at(std::type_index(typeid(EventType)))->insert(std::move(newEventPair));
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
