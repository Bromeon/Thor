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

#include <Thor/Events/Detail/ActionOperations.hpp>

#include <SFML/Window/Window.hpp>

#include <functional>
#include <iterator>
#include <algorithm>

using namespace std::placeholders;


namespace thor
{
namespace detail
{

	bool CompareEvents::operator() (const sf::Event& lhs, const sf::Event& rhs) const
	{
		// Note: Events with the same type member are equivalent.
		// They are differentiated by the filterOut() functions if necessary.
		return lhs.type < rhs.type;
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------


	EventBuffer::EventBuffer(sf::Window& window)
	: mEventSet()
	, mWindow(window)
	, mRealtimeEnabled(true)
	{
	}

	void EventBuffer::pushEvent(const sf::Event& event)
	{
		// Generate realtime actions only if window has the focus. For example, key presses don't invoke callbacks when the
		// window is not focussed.
		switch (event.type)
		{
			case sf::Event::GainedFocus:
				mRealtimeEnabled = true;
				break;

			case sf::Event::LostFocus:
				mRealtimeEnabled = false;
				break;
		}

		// Store event
		mEventSet.insert(event);
	}

	void EventBuffer::clearEvents()
	{
		mEventSet.clear();
	}

	bool EventBuffer::containsEvent(const sf::Event& event) const
	{
		return mEventSet.find(event) != mEventSet.end();
	}

	bool EventBuffer::containsEvent(const sf::Event& event, const ActionNode& filterNode) const
	{
		std::vector<sf::Event> unused;
		return getEvents(event.type, unused, filterNode);
	}

	bool EventBuffer::getEvents(sf::Event::EventType eventType, std::vector<sf::Event>& out) const
	{
		// Create event object acting as key in the multiset
		sf::Event key = {};
		key.type = eventType;

		// Copy found range to output vector
		std::pair<EventSet::const_iterator, EventSet::const_iterator> range = mEventSet.equal_range(key);
		out.insert(out.end(), range.first, range.second);
		return range.first != range.second;
	}

	bool EventBuffer::getEvents(sf::Event::EventType eventType, std::vector<sf::Event>& out, const ActionNode& filterNode) const
	{
		// Collect all events with event.Type == eventType
		std::vector<sf::Event> newEvents;
		getEvents(eventType, newEvents);

		// Copy events that are really equal (e.g. same key) to the end of the output vector
		std::size_t oldSize = out.size();
		std::remove_copy_if(newEvents.begin(), newEvents.end(), std::back_inserter(out), std::bind(&ActionNode::filterOut, &filterNode, _1));
		return oldSize != out.size();
	}

	sf::Window& EventBuffer::getWindow() const
	{
		return mWindow;
	}

	bool EventBuffer::isRealtimeInputEnabled() const
	{
		return mRealtimeEnabled;
	}

	void EventBuffer::pollEvents()
	{
		sf::Event event;

		while (mWindow.pollEvent(event))
			pushEvent(event);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	ActionNode::~ActionNode()
	{
	}

	bool ActionNode::filterOut(const sf::Event&) const
	{
		return false;
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------
	

	bool RealtimeNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Increase counter if derived class (Realtime leaf) returns true
		bool active = isActionActive(buffer);
		out.nbRealtimeTriggers += static_cast<unsigned int>(active);
		return active;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	RealtimeKeyLeaf::RealtimeKeyLeaf(sf::Keyboard::Key key)
	: RealtimeNode()
	, mKey(key)
	{
	}

	bool RealtimeKeyLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.isRealtimeInputEnabled() && sf::Keyboard::isKeyPressed(mKey);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	EventKeyLeaf::EventKeyLeaf(sf::Keyboard::Key key, bool pressed)
	: ActionNode()
	, mKeyEvent()
	{
		mKeyEvent.type = pressed ? sf::Event::KeyPressed : sf::Event::KeyReleased;
		mKeyEvent.key.code = key;
	}

	bool EventKeyLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.containsEvent(mKeyEvent, *this);
	}

	bool EventKeyLeaf::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return buffer.getEvents(mKeyEvent.type, out.eventContainer, *this);
	}

	bool EventKeyLeaf::filterOut(const sf::Event& event) const
	{
		return event.type != mKeyEvent.type || event.key.code != mKeyEvent.key.code;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	RealtimeMouseLeaf::RealtimeMouseLeaf(sf::Mouse::Button mouseButton)
	: RealtimeNode()
	, mMouseButton(mouseButton)
	{
	}

	bool RealtimeMouseLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.isRealtimeInputEnabled() && sf::Mouse::isButtonPressed(mMouseButton);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	EventMouseLeaf::EventMouseLeaf(sf::Mouse::Button mouseButton, bool pressed)
	: ActionNode()
	, mMouseEvent()
	{
		mMouseEvent.type = pressed ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
		mMouseEvent.mouseButton.button = mouseButton;
	}

	bool EventMouseLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.containsEvent(mMouseEvent, *this);
	}

	bool EventMouseLeaf::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return buffer.getEvents(mMouseEvent.type, out.eventContainer, *this);
	}

	bool EventMouseLeaf::filterOut(const sf::Event& event) const
	{
		return event.type != mMouseEvent.type || event.mouseButton.button != mMouseEvent.mouseButton.button;
	}


	// ---------------------------------------------------------------------------------------------------------------------------
	

	RealtimeJoystickLeaf::RealtimeJoystickLeaf(Joystick joystick)
	: RealtimeNode()
	, mJoystick(joystick)
	{
	}

	bool RealtimeJoystickLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.isRealtimeInputEnabled() && sf::Joystick::isButtonPressed(mJoystick.id, mJoystick.button);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	EventJoystickLeaf::EventJoystickLeaf(Joystick joystick, bool pressed)
	: ActionNode()
	, mJoystickEvent()
	{
		mJoystickEvent.type = pressed ? sf::Event::JoystickButtonPressed : sf::Event::JoystickButtonReleased;
		mJoystickEvent.joystickButton.joystickId = joystick.id;
		mJoystickEvent.joystickButton.button = joystick.button;
	}

	bool EventJoystickLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.containsEvent(mJoystickEvent, *this);
	}

	bool EventJoystickLeaf::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return buffer.getEvents(mJoystickEvent.type, out.eventContainer, *this);
	}

	bool EventJoystickLeaf::filterOut(const sf::Event& event) const
	{
		return event.type != mJoystickEvent.type || event.joystickButton.button != mJoystickEvent.joystickButton.button;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	MiscEventLeaf::MiscEventLeaf(sf::Event::EventType eventType)
	: mEvent()
	{
		mEvent.type = eventType;
	}

	bool MiscEventLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.containsEvent(mEvent);
	}

	bool MiscEventLeaf::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return buffer.getEvents(mEvent.type, out.eventContainer);
	}

	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	OrNode::OrNode(ActionNode::CopiedPtr lhs, ActionNode::CopiedPtr rhs)
	: ActionNode()
	, mLhs(std::move(lhs))
	, mRhs(std::move(rhs))
	{
	}

	bool OrNode::isActionActive(const EventBuffer& buffer) const
	{
		return mLhs->isActionActive(buffer) || mRhs->isActionActive(buffer);
	}

	bool OrNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Prevent shortcut semantics of logical OR: If first operand is true, the second's vector isn't filled.
		bool lhsValue = mLhs->isActionActive(buffer, out);
		bool rhsValue = mRhs->isActionActive(buffer, out);

		return lhsValue || rhsValue;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	AndNode::AndNode(ActionNode::CopiedPtr lhs, ActionNode::CopiedPtr rhs)
	: ActionNode()
	, mLhs(std::move(lhs))
	, mRhs(std::move(rhs))
	{
	}

	bool AndNode::isActionActive(const EventBuffer& buffer) const
	{
		return mLhs->isActionActive(buffer) && mRhs->isActionActive(buffer);
	}

	bool AndNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Don't modify out if whole logical AND expression is false -> use temporary result state
		ActionResult tmpResult;

		// If both calls return true, copy events and realtime counter
		if (mLhs->isActionActive(buffer, tmpResult) && mRhs->isActionActive(buffer, tmpResult))
		{
			out.eventContainer.insert(out.eventContainer.end(), tmpResult.eventContainer.begin(), tmpResult.eventContainer.end());
			out.nbRealtimeTriggers += tmpResult.nbRealtimeTriggers;
			return true;
		}
		else
		{
			return false;
		}
	}

} // namespace detail
} // namespace thor
