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

/// @file
/// @brief Class thor::Action

#ifndef THOR_ACTION_HPP
#define THOR_ACTION_HPP

#include <Thor/Input/Detail/ActionOperations.hpp>
#include <Thor/Config.hpp>

#include <functional>


namespace thor
{
namespace detail
{

	class EventBuffer;

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


/// @addtogroup Input
/// @{

/// @brief Class for dynamic actions that are connected with SFML events
/// @details Use this class to build complex constellations of different events (key strokes, pressed mouse buttons, etc.)
///  and associate it with a ActionMap instance.
class THOR_API Action
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:

		/// @brief Type for actions
		///
		enum ActionType
		{
			Hold,			///< Repeated input (e.g. a key that is held down).
			PressOnce,		///< Press events that occur only once (e.g. key pressed).
			ReleaseOnce,	///< Release events that occur only once (e.g. key released).
		};


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Construct key action
		/// @details Creates an action that is in effect when @a key is manipulated. The second parameter specifies whether
		///  KeyPressed events, KeyReleased events or sf::Keyboard::isKeyPressed() act as action source.
		explicit					Action(sf::Keyboard::Key key, ActionType action = Hold);

		/// @brief Construct mouse button action
		/// @details Creates an action that is in effect when @a mouseButton is manipulated. The second parameter specifies whether
		///  MouseButtonPressed events, MouseButtonReleased events or sf::Mouse::isButtonPressed() act as action source.
		explicit					Action(sf::Mouse::Button mouseButton, ActionType action = Hold);

		/// @brief Construct joystick button action
		/// @details Creates an action that is in effect when the joystick button stored in @a joystickState is manipulated.
		///  The second parameter specifies whether JoyButtonPressed events, JoyButtonReleased events or
		///  sf::Joystick::isButtonPressed() act as action source.
		explicit					Action(JoystickButton joystickState, ActionType action = Hold);

		/// @brief Construct joystick axis action
		/// @details Creates an action that is in effect when the absolute value of the joystick axis position exceeds a threshold
		///  (both axis and threshold are stored in @a joystickAxis). The source of the action is sf::Joystick::getAxisPosition()
		///  and not JoystickMoved events. This implies that the action will also be active if the axis remains unchanged in a
		///  position above the threshold.
		explicit					Action(JoystickAxis joystickAxis);

		/// @brief Construct SFML event action
		/// @details Creates an action that is in effect when a SFML event of the type @a eventType is fired.
		explicit					Action(sf::Event::EventType eventType);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Implementation details
	public:
		// Default constructor: Required for ActionMap::operator[] - constructs an uninitialized action
									Action();

		// Tests if the event/real time input constellation in the argument is true for this action
		bool						isActive(const detail::EventBuffer& buffer) const;

		// Test if active and store relevant events
		bool						isActive(const detail::EventBuffer& buffer, detail::ActionResult& out) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Construct from movable pointer to the action's operation
		explicit					Action(detail::ActionNode::CopiedPtr operation);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		detail::ActionNode::CopiedPtr	mOperation;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	/// @cond FriendsAreAnImplementationDetail
	friend Action THOR_API operator|| (const Action& lhs, const Action& rhs);
	friend Action THOR_API operator&& (const Action& lhs, const Action& rhs);
	friend Action THOR_API operator! (const Action& action);
	friend Action THOR_API eventAction(std::function<bool(const sf::Event&)> filter);
	friend Action THOR_API realtimeAction(std::function<bool()> filter);
	/// @endcond
};


/// @relates Action
/// @brief OR operator of two actions: The resulting action is in effect if at least one of @a lhs and @a rhs is active.
/// @details This operator can be used when multiple input types are assigned to an action. For example, an action that can be triggered
///  using either the keyboard or joystick. Another typical example is to combine both modifier keys, such as left and right shift:
/// @code
/// thor::Action shift = thor::Action(sf::Keyboard::LShift) || thor::Action(sf::Keyboard::RShift);
/// thor::Action x(sf::Keyboard::X, thor::Action::PressOnce);
/// thor::Action shiftX = shift && x;
/// @endcode
Action THOR_API				operator|| (const Action& lhs, const Action& rhs);

/// @relates Action
/// @brief AND operator of two actions: The resulting action is in effect if both @a lhs and @a rhs are active.
/// @details This operator is typically used to implement key combinations such as Shift+X. It does not make sense if both operands
///  are event actions, because each of them is only active at one time point, hardly together. Instead, implement modifiers as realtime
///  actions and the actual keys as event actions:
/// @code
/// thor::Action shift = thor::Action(sf::Keyboard::LShift) || thor::Action(sf::Keyboard::RShift);
/// thor::Action x(sf::Keyboard::X, thor::Action::PressOnce);
/// thor::Action shiftX = shift && x;
/// @endcode
Action THOR_API				operator&& (const Action& lhs, const Action& rhs);

/// @relates Action
/// @brief NOT operator of an action: The resulting action is in effect if @a action is not active.
/// @details This operator is rarely needed. It can be used to discriminate two actions, for example if X and Shift+X have different
///  assignments and you don't want Shift+X to trigger also the action assigned to X.
/// @code
/// thor::Action shift = thor::Action(sf::Keyboard::LShift) || thor::Action(sf::Keyboard::RShift);
/// thor::Action x(sf::Keyboard::X, thor::Action::PressOnce);
/// thor::Action shiftX = shift && x;
/// thor::Action onlyX = !shift && x;
/// @endcode
Action THOR_API				operator! (const Action& action);

/// @relates Action
/// @brief Creates a custom action that operates on events
/// @param filter Functor that is called for every event (which is passed as a parameter). It shall return true when the
///  passed event makes the action active.
/// @details Code example: An action that is active when the X key is pressed. This is just an example, in this specific case you
///  should prefer the equivalent expression <i>thor::Action(sf::Keyboard::X, thor::Action::PressOnce)</i>.
/// @code
/// bool isXPressed(const sf::Event& event)
/// {
///     return event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X;
/// }
///
/// thor::Action xPressed = thor::eventAction(&isXPressed);
/// @endcode
Action THOR_API				eventAction(std::function<bool(const sf::Event&)> filter);

/// @relates Action
/// @brief Creates a custom action that operates on realtime input
/// @param filter Functor that is called exactly once per frame, independent of any events. It shall return true when a certain realtime
///  input state should make the action active.
/// @details Code example: An action that is active as long as the X key is held down. This is just an example, in this specific case you
///  should prefer the equivalent expression <i>thor::Action(sf::Keyboard::X, thor::Action::Hold)</i>.
/// @code
/// bool isXHeldDown()
/// {
///     return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
/// }
///
/// thor::Action xHeldDown = thor::realtimeAction(&isXHeldDown);
/// @endcode
Action THOR_API				realtimeAction(std::function<bool()> filter);


/// @}

} // namespace thor

#endif // THOR_ACTION_HPP
