/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
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

template <typename ActionIdentifier>
ActionMap<ActionIdentifier>::ActionMap(sf::Window& window)
: mActionMap()
, mEventBuffer(window)
{
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::update()
{
	mEventBuffer.clearEvents();
	mEventBuffer.pollEvents();
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::pushEvent(const sf::Event& event)
{
	mEventBuffer.pushEvent(event);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::clearEvents()
{
	mEventBuffer.clearEvents();
}

template <typename ActionIdentifier>
Action& ActionMap<ActionIdentifier>::operator[] (const ActionIdentifier& id)
{
	return mActionMap[id];
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::removeAction(const ActionIdentifier& id)
{
	mActionMap.erase(id);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::clearActions()
{
	mActionMap.clear();
}

template <typename ActionIdentifier>
bool ActionMap<ActionIdentifier>::isActive(const ActionIdentifier& id) const
{
	typename Map::const_iterator action = mActionMap.find(id);
	if (action == mActionMap.end())
		return false;

	return action->second.isActive(mEventBuffer);
}

template <typename ActionIdentifier>
void ActionMap<ActionIdentifier>::invokeCallbacks(CallbackSystem& system) const
{
	sf::Window& window = mEventBuffer.getWindow();

	AURORA_CITR_FOREACH(typename Map, mActionMap, actionItr)
	{
		// Check if current action is active, collect additional information in result
		detail::ActionResult result = {};
		if (!actionItr->second.isActive(mEventBuffer, result))
			continue;

		// If at least one realtime constellation triggers this action, invoke callback for it
		if (result.nbRealtimeTriggers > 0)
			system.triggerEvent( ActionContext<ActionIdentifier>(window, NULL, actionItr->first) );

		// Additionally, invoke callback once for every sf::Event
		AURORA_CITR_FOREACH(std::vector<sf::Event>, result.eventContainer, eventItr)
			system.triggerEvent( ActionContext<ActionIdentifier>(window, &*eventItr, actionItr->first) );
	}
}

} // namespace thor
