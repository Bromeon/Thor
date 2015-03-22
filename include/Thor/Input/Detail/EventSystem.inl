/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

namespace thor
{

template <typename Event, typename EventId>
EventSystem<Event, EventId>::EventSystem()
: mListeners()
{
}

template <typename Event, typename EventId>
void EventSystem<Event, EventId>::triggerEvent(const Event& event)
{
	// Import symbol getEventId to qualify for ADL.
	using namespace detail;

	mListeners.call(getEventId(event), event);
}

template <typename Event, typename EventId>
Connection EventSystem<Event, EventId>::connect(const EventId& trigger, std::function<void(const Event&)> unaryListener)
{
	return mListeners.add(trigger, std::move(unaryListener));
}

template <typename Event, typename EventId>
Connection EventSystem<Event, EventId>::connect0(const EventId& trigger, std::function<void()> nullaryListener)
{
	return connect(trigger, std::bind(std::move(nullaryListener)));
}

template <typename Event, typename EventId>
void EventSystem<Event, EventId>::clearConnections(EventId identifier)
{
	mListeners.clear(identifier);
}

template <typename Event, typename EventId>
void EventSystem<Event, EventId>::clearAllConnections()
{
	mListeners.clearAll();
}

// ---------------------------------------------------------------------------------------------------------------------------


namespace detail
{

	// Default implementation for events where the Event type is the same as the EventId type.
	// This declaration must appear after the invocation in call(), since g++ otherwise prefers
	// this function for overload resolution (while the overload for sf::Event isn't considered).
	template <typename Event>
	const Event& getEventId(const Event& event)
	{
		return event;
	}

} // namespace detail
} // namespace thor
