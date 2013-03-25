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

#ifndef THOR_EVENTOPERATIONS_HPP
#define THOR_EVENTOPERATIONS_HPP

#include <Thor/Input/Joystick.hpp>
#include <Thor/Config.hpp>

#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>

#include <set>
#include <vector>


namespace sf
{

	class Window;

} // namespace sf


namespace thor
{
namespace detail
{
	class ActionNode;

	// Functor to compare sf::Event instances
	struct CompareEvents
	{
		bool						operator() (const sf::Event& lhs, const sf::Event& rhs) const;
	};

	// Class that stores events and provides methods for lookup
	class THOR_API EventBuffer : private sf::NonCopyable
	{
		private:
			typedef std::multiset<sf::Event, CompareEvents> EventSet;

		public:
										EventBuffer();

			// Modifiers
			void						pushEvent(const sf::Event& event);
			void						clearEvents();
			void						pollEvents(sf::Window& window);

			// Accessors
			bool						containsEvent(const sf::Event& event) const;
			bool						containsEvent(const sf::Event& event, const ActionNode& filterNode) const;
			bool						getEvents(sf::Event::EventType eventType, std::vector<sf::Event>& out) const;
			bool						getEvents(sf::Event::EventType eventType, std::vector<sf::Event>& out, const ActionNode& filterNode) const;
			bool						isRealtimeInputEnabled() const;

		private:
			EventSet					mEventSet;
			bool						mRealtimeEnabled;
	};
	
	// Structure to collect the sf::Event instances and number of realtime triggers.
	struct ActionResult
	{
		std::vector<sf::Event>			eventContainer;
		unsigned int					nbRealtimeTriggers;
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Operation base class for event actions. The operations are organized in a tree structure.
	class ActionNode
	{
		public:
			typedef aurora::CopiedPtr<ActionNode>	CopiedPtr;

		public:
			virtual						~ActionNode();
			virtual bool				isActionActive(const EventBuffer& buffer) const = 0;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const = 0;
			virtual bool				filterOut(const sf::Event& event) const;
	};

	// Class between ActionNode and concrete Realtime classes
	class RealtimeNode : public ActionNode
	{
		public:
			using						ActionNode::isActionActive;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;
	};

	// Operation node class for keys currently held down
	class RealtimeKeyLeaf : public RealtimeNode
	{
		public:
			explicit					RealtimeKeyLeaf(sf::Keyboard::Key key);
			virtual bool				isActionActive(const EventBuffer& buffer) const;

		private:
			sf::Keyboard::Key				mKey;
	};

	// Operation node class for key events (either pressed or released)
	class EventKeyLeaf : public ActionNode
	{
		public:
										EventKeyLeaf(sf::Keyboard::Key key, bool pressed);
			virtual bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;
			virtual bool				filterOut(const sf::Event& event) const;

		private:
			sf::Event					mKeyEvent;
	};

	// Operation node class for mouse buttons currently held down
	class RealtimeMouseLeaf : public RealtimeNode
	{
		public:
			explicit					RealtimeMouseLeaf(sf::Mouse::Button mouseButton);
			virtual bool				isActionActive(const EventBuffer& buffer) const;

		private:
			sf::Mouse::Button			mMouseButton;
	};

	// Operation node class for mouse button events (either pressed or released)
	class EventMouseLeaf : public ActionNode
	{
		public:
										EventMouseLeaf(sf::Mouse::Button mouseButton, bool pressed);
			virtual bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;
			virtual bool				filterOut(const sf::Event& event) const;

		private:
			sf::Event					mMouseEvent;
	};

	// Operation node class for joystick buttons currently held down
	class RealtimeJoystickLeaf : public RealtimeNode
	{
		public:
			explicit					RealtimeJoystickLeaf(Joystick joystick);
			virtual bool				isActionActive(const EventBuffer& buffer) const;

		private:
			Joystick					mJoystick;
	};

	// Operation node class for joystick button events (either pressed or released)
	class EventJoystickLeaf : public ActionNode
	{
		public:
										EventJoystickLeaf(Joystick joystick, bool pressed);
			virtual bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;
			virtual bool				filterOut(const sf::Event& event) const;

		private:
			sf::Event					mJoystickEvent;
	};

	// Operation node class for other SFML events
	class MiscEventLeaf : public ActionNode
	{
		public:
			explicit					MiscEventLeaf(sf::Event::EventType eventType);
			virtual bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;

		private:
			sf::Event					mEvent;					
	};

	// Logical OR operator
	class OrNode : public ActionNode
	{
		public:
										OrNode(ActionNode::CopiedPtr lhs, ActionNode::CopiedPtr rhs);
			virtual bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;

		private:
			ActionNode::CopiedPtr		mLhs;
			ActionNode::CopiedPtr		mRhs;
	};

	// Logical AND operator
	class AndNode : public ActionNode
	{
		public:
										AndNode(ActionNode::CopiedPtr lhs, ActionNode::CopiedPtr rhs);
			virtual	bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;

		private:
			ActionNode::CopiedPtr		mLhs;
			ActionNode::CopiedPtr		mRhs;
	};

	// Logical NOT operator
	class NotNode : public ActionNode
	{
		public:
			explicit					NotNode(ActionNode::CopiedPtr action);
			virtual	bool				isActionActive(const EventBuffer& buffer) const;
			virtual bool				isActionActive(const EventBuffer& buffer, ActionResult& out) const;

		private:
			ActionNode::CopiedPtr		mAction;
	};

} // namespace detail
} // namespace thor

#endif // THOR_EVENTOPERATIONS_HPP
