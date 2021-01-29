#pragma once
#include "Events.h"

class HandlerFunctionBase
{
public:
	// call member function
	void exec(Event* evnt)
	{
		call(evnt);
	}
private:
	// Implemented by MemberFunctionHandler
	virtual void call(Event* evnt) = 0;
};

template<class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void (T::* MemberFunction)(EventType*);

	MemberFunctionHandler(T* instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction }{};

	void call(Event* evnt)
	{
		// Cast event to the correct type and call member function
		(instance->*memberFunction)(static_cast<EventType*>(evnt));
	}

private:
	T* instance;
	MemberFunction memberFunction;
};