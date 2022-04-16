/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
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
/// @brief Class template thor::ActionContext

#ifndef THOR_ACTIONCONTEXT_HPP
#define THOR_ACTIONCONTEXT_HPP


namespace sf
{

	class Window;
	class Event;

} // namespace sf


namespace thor
{

/// @addtogroup Input
/// @{

/// @brief Structure containing information about the context in which an action has occurred
/// @details This structure aggregates the events and the realtime input that were used to activate a specific action. Its objects
///  are passed to the EventSystem class, which makes it possible for listener functions to get information about the action's trigger
///  (e.g. the sf::Event::KeyPressed event for key presses).
/// @see ActionMap::invokeCallbacks()
template <typename ActionId>
struct ActionContext
{
	// Constructor
	ActionContext(sf::Window* window, const sf::Event* event, const ActionId& actionId)
	: window(window)
	, event(event)
	, actionId(actionId)
	{
	}

	/// @brief Pointer to sf::Window passed to the ActionMap::invokeCallbacks().
	/// @details Use this variable to access the window inside a callback function. This pointer can be @a nullptr if you
	///  didn't specify a window when calling ActionMap::invokeCallbacks().
	sf::Window*					window;

	/// @brief Pointer to a sf::Event that contributed to this action's activation.
	/// @details Do not store the pointer. It is a null pointer if the action is not an event action (triggered by a sf::Event), but a realtime action (triggered
	///  by sf::Mouse, sf::Keyboard or sf::Joystick). The event type behind the pointer depends on the action:
	/// <table>
	///  <tr><th>How was the thor::%Action constructed?</th>							<th>Possible values for @a event->type</th>										</tr>
	///	 <tr><td>Keyboard/mouse/joystick actions constructed with @a PressOnce</td>		<td>KeyPressed, MouseButtonPressed or JoystickButtonPressed</td>				</tr>
	///	 <tr><td>Keyboard/mouse/joystick actions constructed with @a ReleaseOnce</td>	<td>KeyReleased, MouseButtonReleased or JoystickButtonReleased</td>				</tr>
	///	 <tr><td>Keyboard/mouse/joystick actions constructed with @a Hold</td>			<td>The pointer is @a nullptr and thus cannot be dereferenced</td>				</tr>
	///	 <tr><td>Miscellaneous SFML event actions</td>									<td>The sf::Event::EventType specified in the Action constructor</td>			</tr>
	///	 <tr><td>Actions combined with ||, && and ! operators</td>						<td>The one event type in the operands, unless the pointer is @a nullptr</td>	</tr>
	/// </table>
	/// If this context is created for an Action that involves logical operators, then this  @ref event member will represent zero or one involved event actions.
	/// Note that meaningful combinations of logical operators involve at most one %event action and possibly multiple realtime actions that can be active at the same time
	/// (see operator documentation). Example actions are <i>event && realtime</i> or <i>event || event</i>. Thus, only one context will be built for logical combinations,
	/// and at most one event will be stored in it.
	const sf::Event*			event;

	/// @brief Identifier of the action.
	/// @details This is the ID referring to this action, used as argument for ActionMap::operator[] and EventSystem::connect().
	ActionId					actionId;
};

/// @}


// Extracts the ID of an ActionContext object (needed by EventSystem)
template <typename ActionId>
ActionId getEventId(const ActionContext<ActionId>& event)
{
	return event.actionId;
}

} // namespace thor

#endif // THOR_ACTIONCONTEXT_HPP
