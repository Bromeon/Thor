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

#include <Thor/Input/Detail/ActionOperations.hpp>

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


	EventBuffer::EventBuffer()
	: mEventSet()
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

	bool EventBuffer::containsEvent(const sf::Event& event, const ActionNode& filterNode) const
	{
		std::vector<sf::Event> unused;
		return filterEvents(event.type, unused, filterNode);
	}

	bool EventBuffer::filterEvents(sf::Event::EventType eventType, std::vector<sf::Event>& out, const ActionNode& filterNode) const
	{
		// Event as key for std::set
		sf::Event key = {};
		key.type = eventType;

		// Find range of events with event.type == eventType
		auto range = mEventSet.equal_range(key);

		// Variable to check if something was actually inserted (don't look at range, it's not filtered yet)
		std::size_t oldSize = out.size();

		// Copy events that are really equal (e.g. same key) and thus are not filtered out to the end of the output vector
		std::copy_if(range.first, range.second, std::back_inserter(out), std::bind(&ActionNode::filter, &filterNode, _1));
		return oldSize != out.size();
	}

	bool EventBuffer::isRealtimeInputEnabled() const
	{
		return mRealtimeEnabled;
	}

	void EventBuffer::pollEvents(sf::Window& window)
	{
		sf::Event event;

		while (window.pollEvent(event))
			pushEvent(event);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	ActionNode::~ActionNode()
	{
	}

	bool ActionNode::filter(const sf::Event&) const
	{
		return true;
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	EventNode::EventNode()
	: mEvent()
	{
	}

	bool EventNode::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.containsEvent(mEvent, *this);
	}

	bool EventNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return buffer.filterEvents(mEvent.type, out.eventContainer, *this);
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
	: EventNode()
	{
		mEvent.type = pressed ? sf::Event::KeyPressed : sf::Event::KeyReleased;
		mEvent.key.code = key;
	}

	bool EventKeyLeaf::filter(const sf::Event& event) const
	{
		return event.type == mEvent.type && event.key.code == mEvent.key.code;
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
	: EventNode()
	{
		mEvent.type = pressed ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
		mEvent.mouseButton.button = mouseButton;
	}

	bool EventMouseLeaf::filter(const sf::Event& event) const
	{
		return event.type == mEvent.type && event.mouseButton.button == mEvent.mouseButton.button;
	}

	// ---------------------------------------------------------------------------------------------------------------------------
	

	RealtimeJoystickButtonLeaf::RealtimeJoystickButtonLeaf(JoystickButton joystick)
	: RealtimeNode()
	, mJoystick(joystick)
	{
	}

	bool RealtimeJoystickButtonLeaf::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.isRealtimeInputEnabled() && sf::Joystick::isButtonPressed(mJoystick.joystickId, mJoystick.button);
	}

	// ---------------------------------------------------------------------------------------------------------------------------
	

	RealtimeJoystickAxisLeaf::RealtimeJoystickAxisLeaf(JoystickAxis joystick)
	: RealtimeNode()
	, mJoystick(joystick)
	{
	}

	bool RealtimeJoystickAxisLeaf::isActionActive(const EventBuffer& buffer) const
	{
		if (!buffer.isRealtimeInputEnabled())
			return false;

		float axisPos = sf::Joystick::getAxisPosition(mJoystick.joystickId, mJoystick.axis);

		return mJoystick.above && axisPos > mJoystick.threshold
			|| !mJoystick.above && axisPos < mJoystick.threshold;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	EventJoystickLeaf::EventJoystickLeaf(JoystickButton joystick, bool pressed)
	: EventNode()
	{
		mEvent.type = pressed ? sf::Event::JoystickButtonPressed : sf::Event::JoystickButtonReleased;
		mEvent.joystickButton.joystickId = joystick.joystickId;
		mEvent.joystickButton.button = joystick.button;
	}

	bool EventJoystickLeaf::filter(const sf::Event& event) const
	{
		return event.type == mEvent.type && event.joystickButton.button == mEvent.joystickButton.button;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	MiscEventLeaf::MiscEventLeaf(sf::Event::EventType eventType)
	: EventNode()
	{
		mEvent.type = eventType;
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

	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	NotNode::NotNode(ActionNode::CopiedPtr action)
	: ActionNode()
	, mAction(std::move(action))
	{
	}

	bool NotNode::isActionActive(const EventBuffer& buffer) const
	{
		return !mAction->isActionActive(buffer);
	}

	bool NotNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Don't modify if action is active -> use temporary result state
		ActionResult tmpResult;

		if (!mAction->isActionActive(buffer, tmpResult))
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
