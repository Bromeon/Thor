/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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

template <typename ActionId>
void ActionMap<ActionId>::update(sf::Window& window)
{
	mEventBuffer.clearEvents();
	mEventBuffer.pollEvents(window);
}

template <typename ActionId>
void ActionMap<ActionId>::pushEvent(const sf::Event& event)
{
	mEventBuffer.pushEvent(event);
}

template <typename ActionId>
void ActionMap<ActionId>::clearEvents()
{
	mEventBuffer.clearEvents();
}

template <typename ActionId>
Action& ActionMap<ActionId>::operator[] (const ActionId& id)
{
	return mActionMap[id];
}

template <typename ActionId>
void ActionMap<ActionId>::removeAction(const ActionId& id)
{
	mActionMap.erase(id);
}

template <typename ActionId>
void ActionMap<ActionId>::clearActions()
{
	mActionMap.clear();
}

template <typename ActionId>
bool ActionMap<ActionId>::isActive(const ActionId& id) const
{
	typename Map::const_iterator action = mActionMap.find(id);
	if (action == mActionMap.end())
		return false;

	return action->second.isActive(mEventBuffer);
}

template <typename ActionId>
void ActionMap<ActionId>::invokeCallbacks(CallbackSystem& system, sf::Window* window) const
{
	AURORA_FOREACH(const auto& actionPair, mActionMap)
	{
		// Check if current action is active, collect additional information in result
		detail::ActionResult result = {};
		if (!actionPair.second.isActive(mEventBuffer, result))
			continue;

		// Invoke callback once for every sf::Event
		AURORA_FOREACH(const sf::Event& event, result.eventContainer)
			system.triggerEvent( ActionContext<ActionId>(window, &event, actionPair.first) );

		// If at least one realtime constellation triggers this action, invoke callback for it
		if (result.nbRealtimeTriggers > 0)
			system.triggerEvent( ActionContext<ActionId>(window, nullptr, actionPair.first) );
	}
}

} // namespace thor
