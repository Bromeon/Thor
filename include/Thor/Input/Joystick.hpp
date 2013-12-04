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

/// @file
/// @brief Class thor::Joystick

#ifndef THOR_JOYSTICK_HPP
#define THOR_JOYSTICK_HPP

#include <SFML/Window/Joystick.hpp>


namespace thor
{

/// @addtogroup Input
/// @{

/// @brief Contains information about a joystick number and button number
///
struct JoystickButton
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and button property
	///  with the following more expressive syntax:
	/// @code
	/// thor::JoystickButton j = thor::joy(id).button(b);
	/// @endcode
	JoystickButton(unsigned int id, unsigned int button)
	: id(id)
	, button(button)
	{
	}

	unsigned int		id;			///< The joystick number
	unsigned int		button;		///< The joystick button number
};

/// @brief Contains information about a joystick number, an axis and its threshold
///
struct JoystickAxis
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and axis property
	///  with the following more expressive syntax:
	/// @code
	/// thor::JoystickAxis j1 = thor::joy(id).axis(a).above(pos);
	/// thor::JoystickAxis j2 = thor::joy(id).axis(a).below(pos);
	/// @endcode
	JoystickAxis(unsigned int id, sf::Joystick::Axis axis, float threshold, bool above)
	: id(id)
	, axis(axis)
	, threshold(threshold)
	, above(above)
	{
	}

	unsigned int		id;			///< The joystick number
	sf::Joystick::Axis	axis;		///< The joystick axis
	float				threshold;	///< Position threshold above/below which an action is triggered
	bool				above;		///< True if position must be above threshold, false if below
};
/// @}


namespace detail
{

	// Proxy class that allows the joy(id) named parameter syntax
	struct JoyBuilder
	{
		struct Axis
		{
			JoystickAxis above(float threshold)
			{
				return JoystickAxis(id, axis, threshold, true);
			}

			JoystickAxis below(float threshold)
			{
				return JoystickAxis(id, axis, threshold, false);
			}

			sf::Joystick::Axis	axis;
			unsigned int		id;
		};

		explicit JoyBuilder(unsigned int id)
		: id(id)
		{
		}

		JoystickButton button(unsigned int button)
		{
			return JoystickButton(id, button);
		}

		Axis axis(sf::Joystick::Axis axis)
		{
			Axis a;
			a.id = id;
			a.axis = axis;
			return a;
		}

		unsigned int		id;
	};

} // namespace detail

inline detail::JoyBuilder joy(unsigned int joystickId)
{
	return detail::JoyBuilder(joystickId);
}

} // namespace thor

#endif // THOR_JOYSTICK_HPP
