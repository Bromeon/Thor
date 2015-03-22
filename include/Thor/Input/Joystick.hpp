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
/// @brief Classes thor::JoystickButton, thor::JoystickAxis

#ifndef THOR_JOYSTICK_HPP
#define THOR_JOYSTICK_HPP

#include <Thor/Config.hpp>

#include <SFML/Window/Joystick.hpp>


namespace thor
{

/// @addtogroup Input
/// @{

/// @brief Contains information about a joystick number and button number
///
struct THOR_API JoystickButton
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and button property
	///  with the following more expressive syntax:
	/// @code
	/// thor::JoystickButton j = thor::joystick(id).button(b);
	/// @endcode
								JoystickButton(unsigned int joystickId, unsigned int button);

	unsigned int				joystickId;	///< The joystick number
	unsigned int				button;		///< The joystick button number
};

/// @brief Contains information about a joystick number, an axis and its threshold
///
struct THOR_API JoystickAxis
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and axis property
	///  with the following more expressive syntax:
	/// @code
	/// thor::JoystickAxis j1 = thor::joystick(id).axis(a).above(pos);
	/// thor::JoystickAxis j2 = thor::joystick(id).axis(a).below(pos);
	/// @endcode
								JoystickAxis(unsigned int joystickId, sf::Joystick::Axis axis, float threshold, bool above);

	unsigned int				joystickId;	///< The joystick number
	sf::Joystick::Axis			axis;		///< The joystick axis
	float						threshold;	///< Position threshold above/below which an action is triggered
	bool						above;		///< True if position must be above threshold, false if below
};

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


namespace detail
{

	// Proxy class that allows the joy(id) named parameter syntax
	struct THOR_API JoystickBuilder
	{
		struct THOR_API Axis
		{
			JoystickAxis					above(float threshold);
			JoystickAxis					below(float threshold);

			sf::Joystick::Axis				axis;
			unsigned int					joystickId;
		};

		explicit						JoystickBuilder(unsigned int joystickId);
		JoystickButton					button(unsigned int button);
		Axis							axis(sf::Joystick::Axis axis);

		unsigned int					joystickId;
	};

} // namespace detail

detail::JoystickBuilder THOR_API		joystick(unsigned int joystickId);

} // namespace thor

#endif // THOR_JOYSTICK_HPP
